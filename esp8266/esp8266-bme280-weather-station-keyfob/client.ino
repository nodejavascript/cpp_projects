
String clientGet( String url ) {

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  if ( https.begin(*client, url) ) {

    int httpCode = https.GET();

    if (httpCode == HTTP_CODE_OK) {
      return https.getString();
    } else {
      Serial.println( "CLIENT: " + url );
      Serial.println( "httpCode: " + String(httpCode) );
      return "";
    }

    https.end();

  } else {
    Serial.println( "CLIENT: " + url );
    Serial.println( "error: Unable to connect");
    return "";
  }

}

String postRecordToServer( String payload ) {

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  if ( https.begin(*client, graphqlEndpoint) ) {

    https.addHeader("Content-Type", "application/json");

    String quotes = "\"";

    String content = "{ \"accessKey\" : " + quotes + accessKey + quotes + " , \"payload\" : " + quotes + payload + quotes + " }";

    int httpCode = https.POST( content );

    if (httpCode == HTTP_CODE_OK) {
      https.end();
      return String( httpCode ) + "," + https.getString();
    } else {
      https.end();
      return String( httpCode );
    }

  } else {
    return "402";
  }

}
