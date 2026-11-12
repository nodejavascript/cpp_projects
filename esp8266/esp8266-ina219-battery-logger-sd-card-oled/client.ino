
bool resetHttpResponse ( String method, String url ) {
  wifiConnected = isConnected();
  httpMethod = method;
  httpUrl = url;
  httpResponse = "";
  httpError = "";
  httpResponseTime = 0;
  httpCode = 0;
  return true;
}

const String formattedHttpResponse () {
  String ui = "";
  ui += "\n";
  ui += ">>> formattedHttpResponse() <<<";
  ui += "\n";
  ui += "wifiConnected:" + String (wifiConnected);
  ui += "\n";
  ui += "httpMethod:" + httpMethod;
  ui += "\n";
  ui += "httpUrl:" + httpUrl;
  ui += "\n";
  ui += "httpResponse:" + httpResponse;
  ui += "\n";
  ui += "httpError:" + httpError;
  ui += "\n";
  ui += "httpResponseTime:" + String (httpResponseTime);
  ui += "\n";
  ui += "httpCode:" + String (httpCode);
  if (Serial) Serial.println(ui);
  return ui;
}

void executeRequest ( String httpMethod, String url, String accessKey, String payload) {

  resetHttpResponse( httpMethod, url );

  if ( !isConnected() ) httpError = formattedWifiStatus();

  if ( isConnected() ) {

    long startMillis = millis();

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if ( https.begin(*client, url) ) {

      int httpCode;

      if ( httpMethod == "GET" ) httpCode = https.GET();

      if ( httpMethod == "POST" ) {

        https.addHeader("Content-Type", "application/json");
        String content = getContent( accessKey, payload );

        httpCode = https.POST( content );

      }

      httpCode = httpCode;

      if ( httpCode == HTTP_CODE_OK ) httpResponse = https.getString();

      if ( httpCode != HTTP_CODE_OK ) httpError = https.getString();

      https.end();

    } else {

      httpCode = 408;
      httpError = "Connection timeout";

    }

    httpResponseTime = millis() - startMillis;

  }

}

void clientGet( String url ) {
  executeRequest( "GET", url, "", "");
}

void clientPost( String url, String accessKey, String payload ) {
  executeRequest( "POST", url, accessKey, payload );
}

String getContent ( String accessKey, String payload ) {
  payload.trim();
  String quotes = "\"";
  String content = "{ \"accessKey\" : " + quotes + accessKey + quotes + " , \"payload\" : " + quotes + payload + quotes + " }";
  return content;
}
