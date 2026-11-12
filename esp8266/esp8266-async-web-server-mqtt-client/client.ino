
bool resetHttpResponse ( String httpMethod, String url ) {
  httpResponse.httpMethod = httpMethod;
  httpResponse.httpUrl = url;
  httpResponse.httpResponse = "";
  httpResponse.httpError = "";
  httpResponse.httpResponseTime = 0;
  httpResponse.httpCode = 500;
  return true;
}

bool executeRequest ( String httpMethod, String url, String accessKey, String payload) {

  resetHttpResponse( httpMethod, url );

  if ( !isConnected() ) httpResponse.httpError = formattedWifiConnectionStatus();

  if ( isConnected() ) {

    int startMillis = millis();

    //  BearSSL::WiFiClientSecure *client;
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if ( https.begin(*client, url) ) {

      int httpCode;

      if ( httpResponse.httpMethod == "GET" ) httpCode = https.GET();

      if ( httpResponse.httpMethod == "POST" ) {

        https.addHeader("Content-Type", "application/json");
        String content = getContent( accessKey, payload );

        httpCode = https.POST( content );

      }

      httpResponse.httpCode = httpCode;

      if ( httpCode == HTTP_CODE_OK ) httpResponse.httpResponse = https.getString();

      if ( httpCode != HTTP_CODE_OK ) httpResponse.httpError = https.getString();

      https.end();

    } else {

      httpResponse.httpCode = 408;
      httpResponse.httpError = "Connection timeout";

    }

    httpResponse.httpResponseTime = millis() - startMillis;

  }

  if ( httpResponse.httpError.length() > 0 )  createHttpError();

  return true;
}

bool clientGet( String url ) {
  executeRequest( "GET", url, "", "");
  return true;
}

bool clientPost( String url, String accessKey, String payload ) {
  executeRequest( "POST", url, accessKey, payload );
  return true;
}

String getContent ( String accessKey, String payload ) {

  payload.trim();

  String quotes = "\"";
  String content = "{ \"accessKey\" : " + quotes + accessKey + quotes + " , \"payload\" : " + quotes + payload + quotes + " }";
  return content;
}

bool createHttpError () {
  String ui;
  ui += "FAIL: HTTP " + httpResponse.httpMethod;
  ui += "\n\n";
  ui += "Code: " + String( httpResponse.httpCode );
  ui += "\n";

  if (httpResponse.httpResponse) {
    ui += "Response: " + httpResponse.httpResponse;
    ui += "\n";
  }

  if (httpResponse.httpError) {
    ui += "Error: " + httpResponse.httpError;
    ui += "\n";
  }

  ui += "\n";
  ui += httpResponse.httpUrl;

  if (Serial) Serial.println( ui );

  return true;
}
