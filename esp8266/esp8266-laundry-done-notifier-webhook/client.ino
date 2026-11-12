

void executeRequest ( String httpMethod, String url, String content) {

  if ( isConnected() ) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if ( https.begin(*client, url) ) {

      int httpCode;

      if ( httpMethod == "GET" ) httpCode = https.GET();

      if ( httpMethod == "POST" ) {

        https.addHeader("Content-Type", "application/json");

        httpCode = https.POST( content );

      }

      https.end();

    }

  }

}

void clientGet( String url ) {
  executeRequest( "GET", url, "");
}

void clientPostSlack( String message) {
  String url = SLACK_WEBHOOK_URL;
  String content = getContentSlack( message );
  executeRequest( "POST", url, content );
}

String getContentSlack ( String message ) {
  String quotes = "\"";
  String content = "{ \"text\" : " + quotes + message + quotes + " }";
  return content;
}
