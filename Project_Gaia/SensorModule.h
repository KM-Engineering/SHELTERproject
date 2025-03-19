void readDHTSensor() {
  float humidityCorrection = 0;        // BOTH IN CELCIUS
  float temperatureCorrection = -0.5;  //

  TempAndHumidity SkibidiToilet = dht.getTempAndHumidity();

  correctedHumidity = SkibidiToilet.humidity + humidityCorrection;
  correctedTemperature = SkibidiToilet.temperature + temperatureCorrection;
}


void readSoilSensor() {
  soilMoistureValue1 = analogRead(Hygrometer1);
  soilMoisturePercent1 = map(soilMoistureValue1, WATER_SENSOR_VALUE, AIR_SENSOR_VALUE, 100, 0);
  soilMoisturePercent1 = constrain(soilMoisturePercent1, 0, 100);
  delay(100);
  soilMoistureValue2 = analogRead(Hygrometer2);
  soilMoisturePercent2 = map(soilMoistureValue2, WATER_SENSOR_VALUE, AIR_SENSOR_VALUE, 100, 0);
  soilMoisturePercent2 = constrain(soilMoisturePercent2, 0, 100);
  delay(100);
  soilMoistureValue3 = analogRead(Hygrometer3);
  soilMoisturePercent3 = map(soilMoistureValue3, WATER_SENSOR_VALUE, AIR_SENSOR_VALUE, 100, 0);
  soilMoisturePercent3 = constrain(soilMoisturePercent3, 0, 100);
  delay(100);
  soilMoistureValue4 = analogRead(Hygrometer4);
  soilMoisturePercent4 = map(soilMoistureValue4, WATER_SENSOR_VALUE, AIR_SENSOR_VALUE, 100, 0);
  soilMoisturePercent4 = constrain(soilMoisturePercent4, 0, 100);
}
