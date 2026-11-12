
float temperature(){
  return bme.readTemperature();
}

float humidity(){
  return bme.readHumidity();
}

float pressure(){
  return bme.readPressure() / 100.0F;
}

float altitude(){
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}
