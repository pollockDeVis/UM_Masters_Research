
//Including all the Abstraction Layer Headers
#include "df_iot_core.h"
#include "rfid.h"
#include "temp_humidity.h"

//Declaring global variables for the data parameters
float temp;
float humidity;
char rfid_uid[20];                                    //RFID Unique ID Buffer

void setup() {
  Serial.begin(9600);                                   //Setting up Serial 
  rfid_init();                                         //Initializing RFID
  temp_humidity_init();                               //Initializing temp & humidity sensor
  connectWifi("CucianHebat", "dobiqueen109");        //Setting up the WiFi
} //End of Setup

void loop() {
    
  if(rfid_get_id(rfid_uid))                         //Detects if RFID card is available and takes empty buffer for the UID. 
                                                    //Function returns True if card is available
  {
    
    temp = get_temperature();
    humidity = get_humidity();
    
    //Printing out the values to check
    
    Serial.println(rfid_uid);
    Serial.println(temp);
    Serial.println(humidity);

     if( Wifi_OK())                               //Only Upload Data if WiFi is available
     {
      postData(rfid_uid, temp, humidity );        // The input parameters for post data has to follow this order: ID > Temp > Humidity
     } 
    
  }
 
} //End of main loop





  
