/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program to connect to Digital Forest's IoT Core [Hydrone]
   --------------------------------------------------------------------------------------------------------------------
   Digital Forest Sdn Bhd
   Copyright (c) Digital Forest 2020. All rights reserved.
   https://digitalforest.io
   Author: Palok Biswas
   Sensor data are available at https://hydrone.digitalforest.io/admin/login

*/

#include "WiFi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SERIALDEBUG 1 //HTTPS DEBUG. CHANGE VALUE TO 1 TO TURN IN ON
#define USE_SERIAL Serial

/********************CAUTION: DO NOT CHANGE. *****************************************************************************/
//WIFI CREDENTIALS
bool WIFI_ACTIVE = true;
bool WIFI_reconnect_flag = false;
char* ssid;
char* password;
const unsigned long wifi_timeout PROGMEM = 10000; // 10 seconds waiting for connecting to wifi
const unsigned long wifi_reconnect_time PROGMEM = 120000; // 2 min retrying
unsigned long wifi_last_connected_time = millis();

//SERVER DETAILS
const char* host  PROGMEM = "https://hydrone.digitalforest.io/api/v1/readings";

//HTTPS REQUEST CREDENTIALS
const int httpsPort = 443;
const char fingerprint[] PROGMEM = "a8 0e 9c 81 2a a8 e3 0f e8 3b f5 e6 4c 73 7c 07 a4 e7 cc e5";

/********************CAUTION: DO NOT CHANGE. *****************************************************************************/

/**
 * @brief Uploads data to backend using HTTPS POST
 *
 * @param Device ID, Temperature & Humidity
 *
 * @return Void
 */
void postData(String _devID, float _temp, float _humidity)
{
  HTTPClient http;

  http.begin(host);  //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json");             //Specify content-type header
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  root["ph"] = 0;
  root["device_id"] = _devID;
  root["temperature"] = _temp;
  root["turbidity"] = 0;
  root["dissolved_oxygen"] = 0;
  root["chlorophyll"] = 0;
  root["conductivity"] = 0;
  root["pm"] = 0;
  root["humidity"] = _humidity;

  //ADD DATE TIME BASED ON THE FLAG
  int length = root.measureLength();
  char buffer[length + 1];
  root.printTo(buffer, sizeof(buffer));
  int httpResponseCode = http.POST(buffer);   //Send the actual POST request
  if (httpResponseCode > 0) {

    String response = http.getString();                       //Get the response to the request

    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer

  } else {

    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);

  }

  http.end();  //Free resources

}

/**
 * @brief Checks WiFi Status
 *
 * @param None
 *
 * @return Boolean; True for connected & False for Not connected
 */

bool wifiStatusCheck()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * @brief Connects to a WiFi Access Point
 *
 * @param WiFi SSID, WiFi Password
 *
 * @return Void
 */
 
void connectWifi(char* _ssid, char* _password)
{
  ssid = _ssid;
  password = _password;
#if SERIALDEBUG
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
#endif
  for (uint8_t t = 4; t > 0; t--)
  {
#if SERIALDEBUG
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
#endif
    USE_SERIAL.flush();
    delay(100);
  }

  WiFi.begin(_ssid, _password);

  unsigned long lastTime = millis();

  while (wifiStatusCheck() == false && (millis() - lastTime) < wifi_timeout)
  {
    delay(100);
#if SERIALDEBUG
    Serial.println(".");
    Serial.print("Connecting to WiFi, Please Wait: ");
    Serial.println(10000-(millis() - lastTime));
#endif
  }
#if SERIALDEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

/**
 * @brief Checks whether device is connected to WiFi
 *
 * @param None
 *
 * @return Boolean; True for connected & False for Not connected
 */
 
bool Wifi_OK()
{
  WIFI_ACTIVE = wifiStatusCheck();

  if (WIFI_ACTIVE == false)
  {
    if ((millis() - wifi_last_connected_time) > wifi_reconnect_time) // reconnects every 2 minutes || Serial is checked so that it doesn't go into reconnecting the wifi when transaction is taking place
    {
      WIFI_reconnect_flag = true;
      wifi_last_connected_time = millis(); // Readjusting the timer so that it doesn't keep on reconnecting on every loop after 2 min has elapsed
#if SERIALDEBUG
      Serial.print("Connection Lost, Reconnecting");
#endif
      connectWifi(ssid, password);
      WIFI_reconnect_flag = false;
    }
    return false;
  } else {
    return true;
  }

}
