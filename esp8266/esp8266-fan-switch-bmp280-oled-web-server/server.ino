
void createEndpoints () {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("htmlAction")) {
      const AsyncWebParameter* p = request->getParam("htmlAction");
      htmlAction = p->value();
      request->redirect("/");
    }
    request->send(200, "text/html", htmlIndex() );
  });

}

String htmlIndex() {
  String body = "";

  body += htmlSingleBreak();

  body += htmlRowStart();
  body += htmlColStart();

  body += htmlTableStart();

  body += htmlTableKeyValue("switchOn", String(switchOn) );
  body += htmlTableKeyValue("fault", String(fault) );
  body += htmlTableKeyValue("roomTemp", String(roomTemp) );
  body += htmlTableKeyValue("heaterTemp", String(heaterTemp) );
  body += htmlTableKeyValue("fanTempTrigger", String(fanTempTrigger) );
  body += htmlTableKeyValue("minRoomTemp", String(minRoomTemp) );
  body += htmlTableKeyValue("maxRoomTemp", String(maxRoomTemp) );
  body += htmlTableKeyValue("itsHot", String(itsHot) );
  body += htmlTableKeyValue("isFanOn", String(isFanOn) );
  body += htmlTableKeyValue("heaterIsOn", String(heaterIsOn) );

  body += htmlTableEnd();

  body += htmlSingleBreak();
  body += "<a href='/?htmlAction=turnHeaterOn' class='btn btn-primary btn-lg' type='button'>Turn Heater ON</a>";
  body += htmlDoubleBreak();
  body += "<a href='/?htmlAction=turnHeaterOff' class='btn btn-danger btn-lg' type='button'>Turn Heater OFF</a>";
  body += htmlDoubleBreak();
  body += htmlDoubleBreak();
  body += "<a href='/?htmlAction=details' class='btn btn-info btn-lg' type='button'>Details</a>";
  body += htmlColEnd();
  body += htmlRowEnd();

  return htmlHeader() + body + htmlFooter();
}

String htmlSingleBreak() {
  return "<br>";
}
String htmlDoubleBreak() {
  return "<br><br>";
}

String htmlHeader() {
  String htmlheader = "";
  htmlheader += "<!DOCTYPE html><html lang='en'><head><meta http-equiv='refresh' content='5'><meta charset='utf-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  htmlheader += "<title>fan_switch</title>";
  htmlheader += "<link href='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css' rel='stylesheet'></head><body><div class='container-fluid'>";
  return htmlheader;
}

String htmlFooter() {
  return "</div><script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script> <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js'></script></body></html>";
}

String htmlRowStart() {
  return "<div class='row justify-content-md-center'>";
}
String htmlRowEnd() {
  return htmlDivEnd();
}

String htmlColStart() {
  return "<div class='col-md-4 text-center'>";
}

String htmlColEnd() {
  return htmlDivEnd();
}

String htmlTableStart() {
  return "<table class='table table-bordered'>";
}

String htmlTableKeyValue(String key, String value) {
  String row = "";
  row += "<tr>";
  row += "<th>" + key + "</th>";
  row += "<td>" + value + "</td>";
  row += "</tr>";
  return row;
}

String htmlTableEnd() {
  return "</table>";
}

String htmlDivEnd() {
  return "</div>";
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
