#include <SoftwareSerial.h>

SoftwareSerial esp8266Module(10,11); //RX,TX



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("starting..");
  esp8266Module.begin(9600);
  
  delay(5000);// lets the messag
  Serial.println("begin..");
  

}

void loop() {

    String dateTimeVal = "";
    String response = "";
    while (esp8266Module.available()) {
       // Serial.println("~");
        String tmpResp = esp8266Module.readStringUntil('\r');
        if(tmpResp!= "\r\n")
          response += tmpResp;

    }


  //Serial.println(response);

  int startIndex = response.indexOf("<formatted>");

  if (startIndex > 0)
  {
    startIndex += 11;// get end of <formatted> position
      int endIndex = response.indexOf("</formatted>");   
    
    dateTimeVal = response.substring(startIndex,endIndex);
    Serial.print("datetime:");
    Serial.println(dateTimeVal);
    }
 

  
 
  delay(1000);


}
