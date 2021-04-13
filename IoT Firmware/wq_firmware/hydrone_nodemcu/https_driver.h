#define SERIALDEBUG 1 //HTTPS DEBUG. CHANGE VALUE TO 1 TO TURN IN ON
/********************CAUTION: DO NOT CHANGE. *****************************************************************************/
//SERVER DETAILS
const char* host  PROGMEM = "hydrone.digitalforest.io";

//HTTPS REQUEST CREDENTIALS
const int httpsPort = 443;
const char fingerprint[] PROGMEM = "a8 0e 9c 81 2a a8 e3 0f e8 3b f5 e6 4c 73 7c 07 a4 e7 cc e5";

bool SuccessfulHttpRequest(String _client)
{
  //String response  = _client.readStringUntil('{');
  int splitT = _client.indexOf(":");
  String response = _client.substring(0, splitT);
#if SERIALDEBUG
  Serial.print("[CHECK HTTP RESPONSE]HTTPS Response: ");
  Serial.println(response);
  Serial.print("Status code: ");
  Serial.print(response[9]); //9,10,11
  Serial.print(response[10]);
  Serial.println(response[11]);
#endif
//checking for status code 200 which is a successful response. Checking the first number for '2' is sufficient enough. Other error codes start with '4' e.g. 422
  if(response[9] == '2')
    {
      #if SERIALDEBUG
        Serial.println("Successful HTTPS Request");
      #endif
      return true;
    }
  else
  {
    #if SERIALDEBUG
      Serial.println("Unsuccessful HTTPS Request");
    #endif
    return false;
  }
}

bool postCashTransaction_internal(float _temp, float _turb, float _ph, float _doxy) {

  WiFiClientSecure client;
  String endpoint_url   = "/api/v1/readings";
#if SERIALDEBUG
  Serial.print("connecting to ");
  Serial.println(host);
#endif
#if SERIALDEBUG
  Serial.printf("Using fingerprint '%s'\n", fingerprint);
#endif
  client.setFingerprint(fingerprint);

  if (!client.connect(host, httpsPort)) {
#if SERIALDEBUG
    Serial.println("connection failed");
#endif
    //    return;
  }

#if SERIALDEBUG
  Serial.print("requesting URL: ");
  Serial.println(endpoint_url);
#endif
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  root["ph"] = _ph;
  root["temperature"] = _temp;
  root["turbidity"] = _turb;
  root["dissolved_oxygen"] = _doxy;

  //ADD DATE TIME BASED ON THE FLAG
  int length = root.measureLength();
  char buffer[length + 1];
  root.printTo(buffer, sizeof(buffer));



  String requestBody2 = "POST " + endpoint_url + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Content-Type: application/json\r\n" +
                        "Content-Length: " + root.measureLength() +
                        "\r\n\r\n" + buffer;

  client.println(requestBody2);
#if SERIALDEBUG
  //Serial.println(requestBody2);
#endif
  String  line2 = client.readStringUntil('}'); //readString() is significantly slow
#if SERIALDEBUG
  Serial.println(line2);
#endif

 bool statusCheck = SuccessfulHttpRequest(line2);
   //String  line = client.readStringUntil(':'); //readString() is significantly slow
#if SERIALDEBUG
  Serial.println("POST REQ SENT");
#endif

  if(statusCheck == true) return true;
  else return false;
}

bool postCashTransaction(float _temp, float _turb, float _ph, float _doxy) 
{
   //String receivedToken = oauth_bearer_access_token;  
   bool successfulReq = postCashTransaction_internal(_temp, _turb, _ph, _doxy);

  if(successfulReq == true) return true;
  else return false;
}
