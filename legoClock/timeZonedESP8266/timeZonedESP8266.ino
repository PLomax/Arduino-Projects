
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "******";
const char* pwd =  "******";


void setup() {

  Serial.begin(57600);
  WiFi.begin(ssid,pwd);
  Serial.print("Connecting");
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");


  }

  if(WiFi.status() == WL_CONNECTED)
  {        
        Serial.println("\nConnected!");
  }


}

void loop() {
  // wait for WiFi connection
    if(WiFi.status() == WL_CONNECTED)
  {        
    HTTPClient http;



 //   Serial.print("[HTTP] begin...\n");
    http.begin( "http://api.timezonedb.com/v2.1/get-time-zone?key=JERHLBL03EO3&format=xml&by=zone&zone=Australia/Sydney");
   // http.begin("http://192.168.0.4/TEST/index.htm");

 //   Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    String returnCode =String(httpCode);
    
  //  Serial.printf("[HTTP] GET... code: %d\n", httpCode);   

    
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        int index = payload.indexOf("<formatted>");
  
        if (index > 0)
        {
        index += 11;// get end of <formatted> position
        int endIndex = payload.indexOf("</formatted>"); 
        Serial.println(returnCode +"~" + payload.substring(index,endIndex)+"~");
        }
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(30000);
}
