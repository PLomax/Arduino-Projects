/*
      Lego Clock
      Created 22/11/2018
      
      Last updated 22/11/2018
  */


#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial esp8266Module(9,10); //RX,TX
String hourStr = "";
String minuteStr="";
String dateTimeVal = "";
String dateString = "";
int hourInt = 0;
int minuteInt=0;
int lastCheckCount =0;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.


//Stepper Motor Control

//threshold buttons
const int btTop_Hour    = 2;
const int btBottom_Hour = 3;

const int btTop_Minute    = 4;
const int btBottom_Minute = 5;

int btStateTopHour = 0; 
int btStateBottomHour = 0; 

int btStateTopMinute = 0; 
int btStateBottomMinute = 0; 

//Stepper connections
const int PUL_Hour = 8; //define Pulse pin
const int DIR_Hour = 7; //define Direction pin
const int ENA_Hour = 6; //define Enable Pin

const int PUL_Minute = 13; //define Pulse pin
const int DIR_Minute = 12;//define Direction pin
const int ENA_Minute = 11;//define Enable Pin

void setup() {
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
  Serial.begin(9600);
  Serial.println("starting..");
  esp8266Module.begin(57600);
  

  Serial.println("lego clock..");
  
  lcd.setCursor(0,0); //First line
  lcd.print("OK here we go...");
  delay(500);// lets the messag  
  lcd.setCursor(0,0); //First line 
  lcd.print("                 ");

  //Hour Stepper settings
   pinMode(PUL_Hour, OUTPUT);
   pinMode(DIR_Hour, OUTPUT);
   pinMode(ENA_Hour, OUTPUT);
   pinMode(btTop_Hour, INPUT);
   pinMode(btBottom_Hour, INPUT);
   digitalWrite(btTop_Hour, HIGH); //initialise the button to HIGH
   digitalWrite(btBottom_Hour, HIGH);//initialise the button to HIGH
  
  //Minute Stepper settings
   pinMode(PUL_Minute, OUTPUT);
   pinMode(DIR_Minute, OUTPUT);
   pinMode(ENA_Minute, OUTPUT);
   pinMode(btTop_Minute, INPUT);
   pinMode(btBottom_Minute, INPUT);
   digitalWrite(btTop_Minute, HIGH); //initialise the button to HIGH
   digitalWrite(btBottom_Minute, HIGH);//initialise the button to HIGH  
  
}

void loop() {
   delay(500); // for stepper but can be removed
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
        
        if(minuteInt != minuteStr.toInt())
        {
            minuteInt = minuteStr.toInt();
            setMinute();

            //implement minute here
        }         
        
        if(hourInt != hourStr.toInt()) // hour has updated
        {
            hourInt = hourStr.toInt();
            setHour();
            
        }// end of hour test
      }
    }
    
    
   }

 
  lcd.setCursor(6,1); //Second line
  lastCheckStr = String(lastCheckCount); 
  lcd.print("wibble:"+lastCheckStr);
  delay(1000);


}

void setHour()
{
      Serial.println(hourInt);
      if(hourInt >= 24 || hourInt < 0) // should never happen
            {
              reverseHourMotor(24); 
              delay(1000);
            }
             else
             {
               if(hourInt == 0) //midnight
               {
                   reverseHourMotor(24); //move to bottom switch trigger
                   delay(100);
                   forwardHourMotor(1); // move to the first position 
               }
               else
               {
                   if(hourInt < 13)
                   {
                         reverseHourMotor(24); // go to the bottom
                         delay(100);
                         float rotations = hourInt;
                         forwardHourMotor(rotations + 1); //position 1 will be midnight, postion 2 will be 1am etc
                         delay(1000);
                   }
                     else
                   {
                         forwardHourMotor(24); // go to the top
                         delay(100);
                         float rotations = 24-hourInt;
                         reverseHourMotor(rotations); //position 24 will be 11pm, postion 23 will be 10pm etc
                         delay(1000);
                   }
               }
             }// end of else  
}


void setMinute()
{
   Serial.println(minuteInt); 
         if(minuteInt >= 60 || minuteInt < 0) // should never happen
            {
              reverseHourMotor(60); 
              delay(1000);
            }
             else
             {
               if(minuteInt == 0) //60 minutes
               {
                   reverseHourMotor(60); //move to bottom switch trigger
                   delay(100);
                   forwardHourMotor(1); // move to the first position 
               }
               else
               {
                   if(minuteInt < 30)
                   {
                         reverseHourMotor(60); // go to the bottom
                         delay(100);
                         float rotations = minuteInt;
                         forwardHourMotor(rotations + 1); //position 1 will be 60th minute, postion 2 will be 1 min etc
                         delay(1000);
                   }
                     else
                   {
                         forwardHourMotor(60); // go to the top
                         delay(100);
                         float rotations = 60-minuteInt;
                         reverseHourMotor(rotations); //position 60 will be 59 min, postion 59 will be 58 min etc
                         delay(1000);
                   }
               }
             }// end of else 
   
}

void releaseHourMotor()
{
    delay(100);
    digitalWrite(DIR_Hour,LOW);
    digitalWrite(ENA_Hour,LOW);
    digitalWrite(PUL_Hour,LOW);
    delay(100);
}

void releaseMinuteMotor()
{
    delay(100);
    digitalWrite(DIR_Minute,LOW);
    digitalWrite(ENA_Minute,LOW);
    digitalWrite(PUL_Minute,LOW);
    delay(100);
}

void forwardMinuteMotor(float rotations)
{
       long rotationCount = 6400 * rotations;
       
       btStateTopMinute = digitalRead(btTop_Minute);
       Serial.print("\nForward:\ntopState:");
       Serial.println(btStateTopMinute);
       Serial.print("\nrotations:");
       Serial.println(rotationCount);  
            
       for (long i = 0; i<rotationCount; i++)    //Forward 5000 steps
       {
              if (btStateTopMinute == HIGH)//TOP switch triggered exit function
              {
                    Serial.print("TOP switch triggered exit function");
                    break; 
              }

              digitalWrite(DIR_Minute, LOW);
              digitalWrite(ENA_Minute, HIGH);
              digitalWrite(PUL_Minute, HIGH);
              delayMicroseconds(50);
              digitalWrite(PUL_Minute, LOW);
              delayMicroseconds(50);

              btStateTopMinute = digitalRead(btTop_Minute);
       }
       
       releaseMinuteMotor();
}

void reverseMinuteMotor(float rotations)
{

       //6400 = 40mm travel
       //3200 = 20mm travel
       long rotationCount = 6400 * rotations;
       
       btStateBottomMinute = digitalRead(btBottom_Minute);
       Serial.print("\nReverse:\nbottomState:");
       Serial.println(btStateBottomMinute);
       Serial.print("\nrotations:");
       Serial.println(rotationCount);       
       for (long i = 0; i < rotationCount; i++)   //Backward 5000 steps
       {
              if (btStateBottomMinute == HIGH) //BOTTOM switch triggered exit function
              {
                    Serial.print("BOTTOM switch triggered exit function");
                    break;  
              }
              digitalWrite(DIR_Minute, HIGH);
              digitalWrite(ENA_Minute, HIGH);
              digitalWrite(PUL_Minute, HIGH);
              delayMicroseconds(50);
              digitalWrite(PUL_Minute, LOW);
              delayMicroseconds(50);
              
              btStateBottomMinute = digitalRead(btBottom_Minute);
       }

      releaseMinuteMotor();
}

void forwardHourMotor(float rotations)
{
       long rotationCount = 6400 * rotations;
       
       btStateTopHour = digitalRead(btTop_Hour);
       Serial.print("\nForward:\ntopState:");
       Serial.println(btStateTopHour);
       Serial.print("\nrotations:");
       Serial.println(rotationCount);  
            
       for (long i = 0; i<rotationCount; i++)    //Forward 5000 steps
       {
              if (btStateTopHour == HIGH)//TOP switch triggered exit function
              {
                    Serial.print("TOP switch triggered exit function");
                    break; 
              }

              digitalWrite(DIR_Hour, LOW);
              digitalWrite(ENA_Hour, HIGH);
              digitalWrite(PUL_Hour, HIGH);
              delayMicroseconds(50);
              digitalWrite(PUL_Hour, LOW);
              delayMicroseconds(50);

              btStateTopHour = digitalRead(btTop_Hour);
       }
       
       releaseHourMotor();
}


 

void reverseHourMotor(float rotations)
{
       long rotationCount = 6400 * rotations;
       
       btStateBottomHour = digitalRead(btBottom_Hour);
       Serial.print("\nReverse:\nbottomState:");
       Serial.println(btStateBottomHour);
       Serial.print("\nrotations:");
       Serial.println(rotationCount);       
       for (long i = 0; i < rotationCount; i++)   //Backward 5000 steps
       {
              if (btStateBottomHour == HIGH) //BOTTOM switch triggered exit function
              {
                    Serial.print("BOTTOM switch triggered exit function");
                    break;  
              }
              digitalWrite(DIR_Hour, HIGH);
              digitalWrite(ENA_Hour, HIGH);
              digitalWrite(PUL_Hour, HIGH);
              delayMicroseconds(50);
              digitalWrite(PUL_Hour, LOW);
              delayMicroseconds(50);
              
              btStateBottomHour = digitalRead(btBottom_Hour);
       }

      releaseHourMotor();
}
