
void createEndpoints () {

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("fobAction")) {
      const AsyncWebParameter* p = request->getParam("fobAction");
      fobAction = p->value();
      request->redirect("/");
    }
    request->send(200, "text/html", htmlIndex() );
  });

}

String htmlIndex() {
  String body = "";
  body += htmlRowStart();
  body += htmlColStart();

  body += htmlSingleBreak();
  body += htmlLabelValue("Temperature", String( temperature() ) + " °C");
  body += htmlSingleBreak();
  body += htmlLabelValue("Humidity", String( humidity() ) + " %");
  body += htmlSingleBreak();
  body += htmlLabelValue("Pressure", String( pressure() ) + " hPa");
  body += htmlSingleBreak();
  body += htmlLabelValue("Altitude", String( altitude() ) + " m");
  body += htmlSingleBreak();
  body += "<hr>";

  body += "<a href='/?fobAction=unlockSingle' class='btn btn-primary btn-lg' type='button'>Unlock driver</a>";
  body += htmlDoubleBreak();
  body += "<a href='/?fobAction=unlockDouble' class='btn btn-success btn-lg' type='button'>Unlock vehicle</a>";
  body += htmlDoubleBreak();
  body += "<a href='/?fobAction=lockSingle' class='btn btn-warning btn-lg' type='button'>Lock vehicle</a>";
  body += htmlDoubleBreak();
  body += "<a href='/?fobAction=lockDouble' class='btn btn-danger btn-lg' type='button'>Arm vehicle</a>";
  body += htmlDoubleBreak();
  body += htmlDoubleBreak();
  body += "<a href='/?fobAction=soundAlarm' class='btn btn-danger btn-sm' type='button'>PANIC !!!</a>";
  body += htmlColEnd();
  body += htmlRowEnd();

  return htmlHeader() + body + htmlFooter();
}

String htmlLabelValue(String htmlLabel, String htmlValue) {
  String html = "";
//  html += "<div class='row'><div class='col-xs-1'></div><div class='col-xs-5 text-left'>";
  html += htmlLabel + " : ";
//  html += "</div><div class='col-xs-5 text-right'>";
  html += htmlValue;
//  html += "</div><div class='col-xs-1'></div></div>";
  return html;
}

String htmlSingleBreak() {
  return "<br>";
}
String htmlDoubleBreak() {
  return "<br><br>";
}

String htmlHeader() {
  return "<!DOCTYPE html><html lang='en'><head><meta http-equiv='refresh' content='30'><meta charset='utf-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'><title>ESP-12F #1</title> <link href='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css' rel='stylesheet'></head><body><div class='container-fluid'>";
}

String htmlFooter() {
  return "</div><script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script> <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/js/bootstrap.min.js'></script></body></html>";
}

String htmlRowStart() {
   return "<div class='row'>";
}
String htmlRowEnd() {
  return htmlDivEnd();
}

String htmlColStart() {
   return "<div class='col-md-12 text-center'>";
}
String htmlColEnd() {
  return htmlDivEnd();
}

String htmlDivEnd() {
  return "</div>";
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
