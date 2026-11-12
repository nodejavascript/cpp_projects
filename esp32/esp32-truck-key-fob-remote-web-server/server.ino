

void createEndpoints () {

  server.on("/dfsadgadgsd_gadr4524513rdfdarafe/", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("htmlAction")) {
      const AsyncWebParameter* p = request->getParam("htmlAction");
      htmlAction = p->value();
      request->redirect("/dfsadgadgsd_gadr4524513rdfdarafe/");
    }
    request->send(200, "text/html", htmlIndex() );
  });

}

String htmlIndex() {
  String body = "";
  body += htmlRowStart();
  body += htmlColStart();
  body += htmlSingleBreak();

  body += "<a href='/dfsadgadgsd_gadr4524513rdfdarafe/?htmlAction=lock' class='btn btn-primary btn-lg' type='button'>Lock</a>";
  body += htmlDoubleBreak();
  body += "<a href='/dfsadgadgsd_gadr4524513rdfdarafe/?htmlAction=lockAndArm' class='btn btn-primary btn-lg' type='button'>Lock & Arm</a>";
  body += htmlDoubleBreak();

  body += "<a href='/dfsadgadgsd_gadr4524513rdfdarafe/?htmlAction=unlock' class='btn btn-success btn-lg' type='button'>Unlock</a>";
  body += htmlDoubleBreak();
  body += "<a href='/dfsadgadgsd_gadr4524513rdfdarafe/?htmlAction=unlockAll' class='btn btn-success btn-lg' type='button'>Unlock All</a>";
  body += htmlDoubleBreak();

  body += "<a href='/dfsadgadgsd_gadr4524513rdfdarafe/?htmlAction=alarm' class='btn btn-danger btn-lg' type='button'>Alarm</a>";
  body += htmlDoubleBreak();

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
  String stringHeader = "";
  stringHeader += "<!DOCTYPE html><html lang='en'><head><meta http-equiv='refresh' content='30'><meta charset='utf-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1'><title>";
  stringHeader += "Truck Key FOB";
  stringHeader += "</title> <link href='https://stackpath.bootstrapcdn.com/bootstrap/4.2.1/css/bootstrap.min.css' rel='stylesheet'></head><body><div class='container-fluid'>";
  return stringHeader;
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
