
void createEndpoints () {

  server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "OK");
  });

  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (request->url() == "/") {

      JsonDocument doc;
      auto error = deserializeJson(doc, (const char*)data);

      bool hasError = false;
      String errors = "";

      if (postAccessKey.length() > 0 && postPayload.length() > 0) {
          hasError = true;
          errors += "ERROR: Currently posting data";
      }

      if (!error) {

        if (doc.containsKey("accessKey")) {
          String accessKey = doc["accessKey"];
          postAccessKey = accessKey;
        } else {
          hasError = true;
          if (errors.length() > 0) errors += ", ";
          errors += "Required: accessKey";
        }

        if (doc.containsKey("payload")) {
          String payload = doc["payload"];
          postPayload = payload;
        } else {
          hasError = true;
          if (errors.length() > 0) errors += ", ";
          errors += "Required: payload";
        }

      }

      if ( !hasError ) {
        request->send(200, "application/json", "{\"message\":\"Success\"}");
      } else {
        request->send(400, "application/json", "{\"message\":\"" + errors + "\"}");
      }

    }
  });

  server.onNotFound(notFound);
  server.begin();

}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}
