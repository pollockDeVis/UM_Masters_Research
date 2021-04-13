#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include "https_driver.h"
#include "uart_driver.h"

#define SERIALDEBUG 0 //WEBSOCKETS DEBUG. CHANGE VALUE TO 1 TO TURN IN ON
#define USE_SERIAL Serial

//WIFI CREDENTIALS
const char* ssid PROGMEM = "SciFi_2.4Ghz";
const char* password PROGMEM = "df2019ROUTER";
const unsigned long wifi_timeout PROGMEM = 10000; // 10 seconds waiting for connecting to wifi
const unsigned long wifi_reconnect_time PROGMEM = 120000; // 2 min retrying
unsigned long wifi_last_connected_time = millis();

bool WIFI_ACTIVE = true;
bool WIFI_reconnect_flag = false;

ESP8266WiFiMulti WiFiMulti;

char *strings[10];
char *ptr = NULL;
char* temp;
char* turb;
char* ph;
char* doxy;
String sTemp;
String sTurb, spH, sDoxy;
void setup() {
  // put your setup code here, to run once:
USE_SERIAL.begin(9600);
USE_SERIAL.setDebugOutput(false);
/*******WIFI SETUP********************************************************************/  
  connectWifi();
  
}

void loop() {
WIFI_ACTIVE = wifiStatusCheck();
  if(serialDataAvailable())
{
  byte index = 0;
  ptr = strtok(receivedBuffer, ">");
  while(ptr != NULL)
  {
    strings[index] = ptr;
    index++;
    ptr = strtok(NULL, ">");
  }
#if SERIALDEBUG
  Serial.println(index);
  #endif
  
  temp = strings[0];
  turb = strings[1];
  ph = strings[2];
  doxy = strings[3];
  
  sTemp = String(temp);
  sTurb = String(turb);
  spH = String(ph);
  sDoxy = String(doxy);

  if(WIFI_ACTIVE == false)
{
  if((millis() - wifi_last_connected_time) > wifi_reconnect_time) // reconnects every 2 minutes || Serial is checked so that it doesn't go into reconnecting the wifi when transaction is taking place
    {
      WIFI_reconnect_flag = true;
      wifi_last_connected_time = millis(); // Readjusting the timer so that it doesn't keep on reconnecting on every loop after 2 min has elapsed
    #if SERIALDEBUG
      Serial.print("Connection Lost, Reconnecting");
    #endif
      connectWifi();
        WIFI_reconnect_flag = false;
    }   
}
else{
  float fTemp = sTemp.toFloat();
  float fTurb = sTurb.toFloat();
  float fpH = spH.toFloat();
  float fDoxy = sDoxy.toFloat();
  postCashTransaction(fTemp,fTurb,fpH, fDoxy);
  delay(1000);
    }
}
}
bool wifiStatusCheck()
{
  if(WiFiMulti.run() == WL_CONNECTED)
  {         
    return true;
  }
  else
  {    
    return false;
  }
}
void connectWifi()
{
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
  WiFiMulti.addAP(ssid, password);
  unsigned long lastTime = millis();
  
  while (wifiStatusCheck() == false && (millis()-lastTime) < wifi_timeout) 
  {
    delay(100);
    #if SERIALDEBUG
      Serial.println(".");
      Serial.print("Time: ");
      Serial.println(millis()-lastTime);
    #endif
  }
  #if SERIALDEBUG
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  #endif
}
