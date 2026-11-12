# Security and credentials

**The rule.** No credential is ever committed. Every project that needs one reads
a git-ignored `config.h`, and the tracked `config.h.example` holds placeholders.

## The four that leaked, and how

Four real credentials reached this repository before anyone noticed. Recording
them is the point — each one taught something the next scanner had to catch.

| What | Where | How it was missed at first |
|---|---|---|
| Slack incoming webhook | laundry notifier, fan switch | It is a URL, not a `password=`, so keyword scans walked past it |
| Three data-server API keys | ESP32 power monitor, keyfob, battery logger | Assigned to `char*` / `String`, not `const char*`, so declaration-pattern scans walked past them |
| Home Wi-Fi password, twice | camera streamer, access point bridge | Present from the first commit; found only when the archive was audited |

**A value published once cannot be unpublished.** Removing it from the repository
stops the next leak; it does not undo the first.

### What was done about each one, on 13 November 2026

- **The Slack webhook is revoked.** George deleted it. The value that appears in
  the retired history is dead and authenticates nothing.
- **The three data-server API keys are deleted, not rotated.** They were removed
  from every `config.h` on disk and stood in for by a placeholder. The server they
  belonged to no longer resolves — it has no DNS record at all — so there is no
  host left to authenticate against. Its address and every fragment of its keys
  were also stripped from this repository, because a partly-quoted credential is
  still a credential.
- **The Wi-Fi password is still to be changed** at the router if that network is
  still in use. It is the one item on this list that is both live and George's.

### The history was rebuilt, because a force-push does not erase anything

A forced push moves the branch. It does not remove the old commits from GitHub —
they stay fetchable by their SHA to anyone with access. That was tested, not
assumed: five superseded commits were still resolvable after three force-pushes,
and one of them carried the original six-commit history with the credentials
inline in six sketches. An independent scan of that history found **eleven**
leaks where the repository scan finds none.

So the repository was replaced rather than rewritten. This repository was created
fresh, with no ancestor, so **no secret has ever existed in its history** — and the
old one was deleted on 13 November 2026, which is why the history above is a record
of a repository that no longer exists rather than a warning about this one.

The same history also sat on this machine as a packed git clone under
`~/copilot-quarantine-20261112-195442-oldcommits/repo`. A packed object store cannot
be redacted, only removed; the extracted trees beside it have already had every full
key and webhook token replaced.

### Verified, with two independent scanners

Neither this repository's own scanner nor gitleaks is trusted alone; they run
against each other. On a fresh clone of this repository:

```
commits: 1        files: 128        projects: 27
gitleaks over the full history: no leaks found
tools/scan-secrets.py:          clean, 128 files, no credential found
none of the old commit SHAs resolve here
```

## What protects this repository now

| Layer | What it does | Where |
|---|---|---|
| `.gitignore` | `config.h`, `config*.h`, `.env*`, `secrets.h`, `*.pem`, `*.key`, `*.p12`, `*.jks` — with `!config.h.example` re-allowed | `.gitignore` |
| **Pre-commit hook** | Refuses a commit whose staged files contain a credential | `tools/scan-secrets.py --install-hook` |
| **Repository scan** | Same rules over every tracked file | `python3 tools/scan-secrets.py` |
| **CI on every push** | gitleaks over the full history **and** the repository scan, in two separate jobs | `.github/workflows/secret-scan.yml` |

### The scanner looks for shapes, not for a list

A list of known-bad values only catches what has already burned you. The scanner
checks for the *shape* of a credential:

- provider formats — Slack webhooks, GitHub tokens, AWS, Google, Stripe, OpenAI/Anthropic, JWTs, private keys
- connection strings carrying a password, and credentials inside a URL
- any secret-named variable or `#define` assigned a literal
- **any quoted, high-entropy string of 16–120 characters**, which is what caught the three API keys

`#define` lines are explicitly scanned. The first version skipped every line
starting with `#` — exactly where a firmware secret hides.

Placeholders are recognised by name (`your-`, `example`, `REPLACE`,
`mqtt-password`, `change-this-token`, …) and allowed.

## Install the hook on a fresh clone

Hooks are not carried by `git clone`. After cloning, run this once:

```bash
python3 tools/scan-secrets.py --install-hook
```

That is the only setup step. Everything else — `config.h`, the `.gitignore`
rules, the CI workflow — travels with the repository.

## If the scanner blocks you

It is right more often than you are. Move the value into `config.h`:

```bash
cd esp8266/esp8266-bme280-weather-station-keyfob
cp config.h.example config.h
$EDITOR config.h          # the real value goes here
```

`git commit --no-verify` skips the hook. If you use it, rotate whatever you
committed, because the hook was telling you something true.

In a document that deliberately shows a credential's shape to teach what not to
do, add `scan-secrets:ignore` to that line. It is honoured, and it should be
used rarely enough that seeing it means something.

## Never commit

```
config.h          .env / .env.*        secrets.h
*.pem  *.key      *.p12  *.pfx          *.jks  *.keystore
```

## Reporting

This repository is **public**. Anything found here that should not be here — a
credential, a private address, a file that should never have left a machine —
report it to George directly, or open an issue without repeating the value.

## Honest limits

- **The scanner cannot catch a credential nobody has a pattern for** — a
  short numeric PIN, a passphrase made of dictionary words, a token that looks
  like prose. It raises the cost of a mistake; it does not remove it.
- **A squash is not an erasure.** Rewriting history removes a value from
  reachable commits. The old objects persist until the repository is deleted —
  which is now what happened. Kept here because it is the reason this repository
  was rebuilt from nothing rather than squashed:
  and anyone holding an old clone still has them.
- **Deleting the repository is not an erasure either.** It is the strongest
  self-service action available, and still weaker than rotating the credential.
