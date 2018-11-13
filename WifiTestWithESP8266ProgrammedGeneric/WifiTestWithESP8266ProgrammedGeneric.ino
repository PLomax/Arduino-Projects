#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial esp8266Module(10,11); //RX,TX



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("starting..");
  esp8266Module.begin(9600);
  delay(1000);// lets the messag
  Serial.println("begin..");
  

}

void loop() {

  
    String response = "";
    while (esp8266Module.available()) {
       // Serial.println("~");
        String tmpResp = esp8266Module.readStringUntil('\r');
        if(tmpResp!= "\r\n")
          response += tmpResp;

    }


  Serial.println(response);
    
   
  
 
  delay(800);


}
