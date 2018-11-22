#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial esp8266Module(10,11); //RX,TX
String hourStr = "";
String minuteStr="";
String dateTimeVal = "";
String dateString = "";
int hourInt = 0;
int mintueInt=0;
int lastCheckCount =0;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.

void setup() {
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
  Serial.begin(9600);
  Serial.println("starting..");
  esp8266Module.begin(57600);
  

  Serial.println("basic clock..");
  
  lcd.setCursor(0,0); //First line
  lcd.print("OK here we go...");
  delay(500);// lets the messag  
  lcd.setCursor(0,0); //First line 
  lcd.print("                 ");
}

void loop() {

   lastCheckCount++;
   String lastCheckStr = "";    
   String response = "";
    while (esp8266Module.available()) {
       // Serial.println("~");
        String tmpResp = esp8266Module.readStringUntil('\r');
        if(tmpResp!= "\r\n")
          response += tmpResp;

    }

    Serial.println(response);
  
  int index = response.indexOf("~"); //start of timedate string
  
  if (index >= 0)
  {
      int endIndex = response.lastIndexOf("~");   
    
    dateTimeVal = response.substring(index+1,endIndex);
    Serial.println(dateTimeVal);

    index = dateTimeVal.indexOf(' ');
    if(index > 0)
    {
      dateString = dateTimeVal.substring(0,index);
      Serial.println(dateString);

      lcd.setCursor(0,0); //First line
      lcd.print(dateString);

      
      String timeString =  dateTimeVal.substring(index+1);

      
      Serial.println(timeString);
      
      int colonIndex = timeString.indexOf(':');
      if(colonIndex > 0)
      {
        hourStr = timeString.substring(0,colonIndex);
        minuteStr = timeString.substring(colonIndex+1,colonIndex+3);
        Serial.println(hourStr);
        Serial.println(minuteStr);
        lcd.setCursor(0,1); //Second line
        lcd.print(hourStr+":"+minuteStr);
        lastCheckCount =0;
        lastCheckStr = String(lastCheckCount); 
        lcd.setCursor(6,1); //Second line
        lcd.print("wibble:   ");
        lcd.print("wibble:"+lastCheckStr);
        hourInt = hourStr.toInt();
        minuteInt = minuteStr.toInt();
      }
    }
    
    
   }

 
  lcd.setCursor(6,1); //Second line
  lastCheckStr = String(lastCheckCount); 
  lcd.print("wibble:"+lastCheckStr);
  delay(1000);


}