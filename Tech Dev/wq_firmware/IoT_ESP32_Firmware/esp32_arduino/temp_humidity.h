/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program for DHT11 Sensor Abstraction and temperature & humidity value extraction
   --------------------------------------------------------------------------------------------------------------------
   Digital Forest Sdn Bhd
   Copyright (c) Digital Forest 2020. All rights reserved.
   https://digitalforest.io
   Author: Palok Biswas
   Sensor data are available at https://hydrone.digitalforest.io/admin/login
*/


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 17     // Digital pin connected to the DHT sensor 

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void temp_humidity_init()
{
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

float get_temperature()
{
 // delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    float _temp = event.temperature;
//    Serial.print(F("Temperature: "));
//    Serial.print( _temp);
//    Serial.println(F("°C"));
    return  _temp;
  }
}

float get_humidity()
{
  //delay(delayMS);
   // Get humidity event and print its value.
     sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    float _humidity = event.relative_humidity;
//    Serial.print(F("Humidity: "));
//    Serial.print(_humidity);
//    Serial.println(F("%"));
    return _humidity;
  }
}
