
bool executeRequest ( String httpMethod, String url, String payload) {

//  unsigned long responseTime;
  long responseCode = 500;

  HTTPClient http;

  if ( !isConnected() ) {
    responseCode = 500;
    return false;
  }

  if ( isConnected() ) {

    if ( http.begin(url) ) {

      http.setConnectTimeout(5000);

      if ( httpMethod == "GET" ) responseCode = http.GET();

      if ( httpMethod == "POST" ) {

        http.addHeader("Content-Type", "application/json");
        String content = getContent( payload );
        responseCode = http.POST( content );
      }

      if(responseCode > 0) {

        if(responseCode == HTTP_CODE_OK) {
          if (http.getString()) {

          }
        }
      }

      http.end();
    }

  } else {
    responseCode = 408;
  }

  Serial.println("responseCode" + responseCode);

//
//  ui += "responseCode: " + String(responseCode);
//  ui += "\n";
//  ui += "responseMessage: " + responseMessage;
//  ui += "\n";
//  ui += "responseTime: " + String(responseTime);
//
//
  return true;
}

bool clientGet( String url ) {
  executeRequest( "GET", url, "");
  return true;
}

bool clientPost( String url, String payload ) {
  executeRequest( "POST", url, payload );
  return true;
}

String getContent ( String payload ) {
  String content;
  payload.trim();
  StaticJsonDocument<500> doc;
  doc["accessKey"] = accessKey;
  doc["payload"] = payload;
  serializeJson(doc, content);
  return content;
}
