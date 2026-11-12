#!/usr/bin/env python3
"""scan-secrets.py — refuse a commit that carries a credential.

WHY THIS EXISTS
    Four real credentials reached this repository before anyone noticed: a
    Slack incoming-webhook URL, three API keys for a data server, a Wi-Fi
    password and an MQTT login. Every one was found by hand, after it had
    already been pushed. A scanner that runs before the commit is the only
    version of this that works, because a value published once cannot be
    unpublished.

WHAT IT DOES
    Reads every file git would commit and looks for the SHAPE of a credential
    rather than for a list of known values — a list only catches what you have
    already been burned by. Placeholders are recognised and allowed.

USAGE
    python3 tools/scan-secrets.py                 # scan the tracked tree
    python3 tools/scan-secrets.py <path>...       # scan specific paths
    python3 tools/scan-secrets.py --staged        # scan what is staged
    python3 tools/scan-secrets.py --install-hook  # install the pre-commit hook

EXIT CODES
    0  clean
    1  a credential was found — DO NOT COMMIT
    2  could not run (never read this as clean)
"""

from __future__ import annotations

import math
import collections
import os
import re
import subprocess
import sys
from pathlib import Path

# A placeholder is a value that is meant to be read, not used.
PLACEHOLDER = re.compile(
    r"(?i)^("
    r"your[-_]|test[-_]|example|placeholder|change[-_]?(me|this)|replace"
    r"|redacted|xxx+|<[^>]*>|\{\{.*\}\}|\$\{.*\}"
    r"|wifissid|wifipassword|mqttpassword|mqqtuser|mqtthostname|mqtt-password|mqtt-user"
    r"|at-least-|your-network|REPLACE|none|null|todo|0123456789"
    r"|esp-access-point|esp-access|dummy|sample|fake|foo|bar|abc123"
    r")"
)

# (name, regex, what it is)
PATTERNS: list[tuple[str, re.Pattern, str]] = [
    ("slack-webhook", re.compile(r"hooks\.slack\.com/services/(?![A-Za-z0-9/]*REPLACE)[A-Za-z0-9/]{20,}"), "Slack incoming webhook"),
    ("private-key", re.compile(r"-----BEGIN [A-Z ]*PRIVATE KEY-----"), "private key"),
    ("github-token", re.compile(r"\b(gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{30,})"), "GitHub token"),
    ("openai-anthropic", re.compile(r"\bsk-(proj-|ant-)?[A-Za-z0-9_\-]{20,}"), "model API key"),
    ("aws-access", re.compile(r"\bAKIA[0-9A-Z]{16}\b"), "AWS access key"),
    ("google-api", re.compile(r"\bAIza[0-9A-Za-z_\-]{35}\b"), "Google API key"),
    ("stripe", re.compile(r"\b[sr]k_(live|test)_[A-Za-z0-9]{20,}"), "Stripe key"),
    ("jwt", re.compile(r"\beyJ[A-Za-z0-9_\-]{10,}\.[A-Za-z0-9_\-]{10,}\.[A-Za-z0-9_\-]{10,}"), "JWT"),
    ("connstring", re.compile(r"\b(mongodb(\+srv)?|postgres(ql)?|mysql|redis|amqp)://[^\s:@/\"']+:[^\s@/\"']{4,}@"),
     "connection string with a password"),
    ("credentials-in-url", re.compile(r"https?://[^\s:@/\"']{3,}:[^\s@/\"']{4,}@"), "credentials in a URL"),
    ("bearer", re.compile(r'(?i)\b(bearer|authorization)["\s:=]+[A-Za-z0-9_\-\.]{20,}'), "authorization header"),
    ("secret-assign", re.compile(
        r'(?i)\b(\w*(password|passwd|pwd|secret|token|apikey|api_key|accesskey|authkey)\w*)\s*[:=]\s*"([^"\n]{8,})"'),
     "secret assigned to a literal"),
    ("define-secret", re.compile(r'(?i)#define\s+(\w*(PASS|PWD|SECRET|TOKEN|KEY)\w*)\s+"([^"\n]{6,})"'),
     "secret in a #define"),
]

SKIP_EXT = {".jpg", ".jpeg", ".png", ".gif", ".ico", ".webp", ".pdf", ".zip",
            ".gz", ".bin", ".elf", ".hex", ".map", ".svg", ".woff", ".ttf"}
SKIP_DIRS = {".git", "build", "node_modules", "__pycache__"}
# The scanner carries the patterns themselves, so it always matches its own rules.
SELF = Path(__file__).resolve() if "__file__" in globals() else None
MAX_BYTES = 1_000_000


def entropy(text: str) -> float:
    if not text:
        return 0.0
    counts = collections.Counter(text)
    n = len(text)
    return -sum((c / n) * math.log2(c / n) for c in counts.values())


def looks_random(value: str) -> bool:
    """A machine-generated string: mixed case, digits, and unusual for prose."""
    if len(value) < 16 or len(value) > 120:
        return False
    if any(ch in value for ch in " /@:#?&=<>_"):
        return False
    # A string of camelCase words is a name, not a key: busVoltage, loadMilliAmps.
    if re.fullmatch(r"[A-Za-z]+", value.replace("-", "")) and value.count("_") == 0:
        words = re.findall(r"[A-Z][a-z]+|[a-z]+", value)
        if len(words) >= 2 and all(len(w) >= 3 for w in words):
            return False
    has_lower = any(c.islower() for c in value)
    has_upper = any(c.isupper() for c in value)
    has_digit = any(c.isdigit() for c in value)
    return has_lower and has_upper and has_digit and entropy(value) > 3.7


def is_placeholder(value: str) -> bool:
    return bool(PLACEHOLDER.match(value.strip()))


def scan_text(text: str, name: str) -> list[tuple[str, str, int, str]]:
    """Return (kind, file, line number, excerpt) for every finding."""
    out: list[tuple[str, str, int, str]] = []
    for lineno, line in enumerate(text.splitlines(), 1):
        if len(line) > 600:
            continue
        stripped = line.strip()
        # Skip comments, but never a preprocessor line — "#define WIFI_PASS ..."
        # is exactly where a secret hides, and the first version of this skipped it.
        if stripped.startswith(("//", "*", "/*")):
            continue
        if stripped.startswith("#") and not stripped.startswith("#define"):
            continue
        # An explicit, deliberate suppression. Used only where a document shows a
        # credential's SHAPE to teach what not to do — never on real code.
        if "scan-secrets:ignore" in line:
            continue

        for kind, rx, _desc in PATTERNS:
            for m in rx.finditer(line):
                # The third group, when present, is the candidate value itself.
                value = m.group(m.lastindex) if m.lastindex else m.group(0)
                if is_placeholder(str(value)):
                    continue
                out.append((kind, name, lineno, stripped[:140]))
                break

        # Independent check: any quoted string that simply looks like a key.
        for qm in re.finditer(r'"([^"\s]{16,120})"', line):
            value = qm.group(1)
            if "://" in value or is_placeholder(value):
                continue
            if looks_random(value):
                out.append(("high-entropy-string", name, lineno, stripped[:140]))
                break
    return out


def tracked_files() -> list[Path]:
    try:
        res = subprocess.run(["git", "ls-files", "-z"], capture_output=True, check=True)
        names = [n for n in res.stdout.decode().split("\0") if n]
        return [Path(n) for n in names if Path(n).exists()]
    except (subprocess.CalledProcessError, FileNotFoundError):
        return [p for p in Path(".").rglob("*")
                if p.is_file() and not (set(p.parts) & SKIP_DIRS)]


def staged_files() -> list[Path]:
    try:
        res = subprocess.run(["git", "diff", "--cached", "--name-only", "--diff-filter=ACMR", "-z"],
                             capture_output=True, check=True)
        return [Path(n) for n in res.stdout.decode().split("\0") if n and Path(n).exists()]
    except (subprocess.CalledProcessError, FileNotFoundError):
        return []


HOOK = """#!/bin/sh
# Refuse a commit that carries a credential. Installed by tools/scan-secrets.py.
# Bypass with: git commit --no-verify   (and then rotate whatever you committed)
python3 "$(git rev-parse --show-toplevel)/tools/scan-secrets.py" --staged || {
    echo ""
    echo "COMMIT REFUSED: a credential was found in the staged files."
    echo "Move it into config.h (git-ignored) and leave a placeholder in config.h.example."
    exit 1
}
"""


def install_hook() -> int:
    try:
        root = subprocess.run(["git", "rev-parse", "--show-toplevel"],
                              capture_output=True, check=True, text=True).stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("not inside a git repository", file=sys.stderr)
        return 2
    hook = Path(root) / ".git" / "hooks" / "pre-commit"
    hook.parent.mkdir(parents=True, exist_ok=True)
    hook.write_text(HOOK)
    hook.chmod(0o755)
    print(f"pre-commit hook installed at {hook}")
    return 0


def main(argv: list[str]) -> int:
    if "--install-hook" in argv:
        return install_hook()

    staged = "--staged" in argv
    explicit = [a for a in argv[1:] if not a.startswith("--")]

    if explicit:
        targets = [Path(p) for p in explicit]
    elif staged:
        targets = staged_files()
    else:
        targets = tracked_files()

    findings: list[tuple[str, str, int, str]] = []
    for path in targets:
        if path.suffix.lower() in SKIP_EXT or set(path.parts) & SKIP_DIRS:
            continue
        if SELF is not None and path.resolve() == SELF:
            continue
        try:
            if path.stat().st_size > MAX_BYTES:
                continue
            text = path.read_text(errors="ignore")
        except OSError:
            continue
        findings.extend(scan_text(text, str(path)))

    if not findings:
        print(f"scan-secrets: clean — {len(targets)} file(s) checked, no credential found")
        return 0

    print(f"scan-secrets: {len(findings)} FINDING(S) — DO NOT COMMIT\n", file=sys.stderr)
    for kind, name, lineno, excerpt in findings:
        print(f"  [{kind}] {name}:{lineno}", file=sys.stderr)
        print(f"      {excerpt}", file=sys.stderr)
    print("\nMove each value into config.h and leave a placeholder in config.h.example.",
          file=sys.stderr)
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
