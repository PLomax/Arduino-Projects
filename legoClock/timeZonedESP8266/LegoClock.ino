/*
      Lego Clock
      Created 22/11/2018
      
      Last updated 15/12/2018
  */


#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial esp8266Module(12,13); //RX,TX
String hourStr = "";
String minuteStr="";
String dateTimeVal = "";
String dateString = "";
int hourInt = 0;
int minuteInt=0;
int lastCheckCount =0;
bool intialised = false;

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.


//Stepper Motor Control

//threshold buttons
const int btTop_Hour    = 9;
const int btBottom_Hour = 8;

const int btTop_Minute    = 11;
const int btBottom_Minute = 10;

int btStateTopHour = 0; 
int btStateBottomHour = 0; 

int btStateTopMinute = 0; 
int btStateBottomMinute = 0; 

//Stepper connections
const int PUL_Hour = 38; //define Pulse pin
const int DIR_Hour = 40; //define Direction pin
const int ENA_Hour = 42; //define Enable Pin

const int PUL_Minute = 39; //define Pulse pin
const int DIR_Minute = 41;//define Direction pin
const int ENA_Minute = 43;//define Enable Pin

//LEDs
const int LED_Top_HourPin = 30;
const int LED_Top_MinutePin = 31;
const int LED_Bottom_HourPin = 26;
const int LED_Bottom_MinutePin = 27;

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
   pinMode(LED_Top_HourPin, OUTPUT);
   pinMode(LED_Top_MinutePin, OUTPUT);
   pinMode(LED_Bottom_HourPin, OUTPUT);
   pinMode(LED_Bottom_MinutePin, OUTPUT);
   
   digitalWrite(LED_Top_HourPin, HIGH);  
   digitalWrite(LED_Top_MinutePin, HIGH);  

   digitalWrite(LED_Bottom_HourPin, HIGH);  
   digitalWrite(LED_Bottom_MinutePin, HIGH);  
}

void loop() {
   delay(500); // for stepper but can be removed
   lastCheckCount++;
   String lastCheckStr = "";    
   String response = "";
   while (esp8266Module.available()) {
        Serial.println("~");
        String tmpResp = esp8266Module.readStringUntil('\r');
        if(tmpResp!= "\r\n")
          response += tmpResp;
    }

   Serial.println(response);
   
   if(response.indexOf("Refused") > 0) //Connection refused
    {
      ledConnectionRefused();
    }
      
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
      
      String timeString =  dateTimeVal.substring(index + 1);
      
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
        }         
        
        if(hourInt != hourStr.toInt()) // hour has updated
        {
            hourInt = hourStr.toInt();
            setHour();
        }// end of hour test

        intialised = true; // clock has been set at least once
      }
    }
  
   }

 
  lcd.setCursor(6,1); //Second line
  lastCheckStr = String(lastCheckCount); 
  lcd.print("wibble:"+lastCheckStr);
  delay(1000);
}

void ledConnectionRefused()
{
  for(int i=0;i < 20; i++)
  {
    digitalWrite(LED_Top_MinutePin, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_Top_HourPin, HIGH);     
    delay(300);                       // wait for a second
    digitalWrite(LED_Top_MinutePin, HIGH);    // turn the LED off by making the voltage LOW
    digitalWrite(LED_Top_HourPin, LOW); 
    delay(300);    
  }
   
  digitalWrite(LED_Top_HourPin, HIGH);  
     
}


void ledBlink(int pinID, int blinkCount)
{
  Serial.println(""); 
  for(int i=0;i < blinkCount; i++)
  {
    digitalWrite(pinID, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(pinID, HIGH);    // turn the LED off by making the voltage LOW
    delay(200);    
    Serial.print("*");
  }
  Serial.println("");
  
}


void setHour()
{
      ledBlink(LED_Top_HourPin,hourInt);
      Serial.print("\nSet Hour:");
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
                   if(hourInt <= 12)
                   {
                        if(!intialised || hourInt == 10 || hourInt == 12) // dont make a noise early in the morning
                        { 
                         reverseHourMotor(24); // go to the bottom
                         delay(100);
                         float rotations = hourInt;
                         forwardHourMotor(rotations + 1); //position 1 will be midnight, postion 2 will be 1am etc
                         delay(1000);
                        }
                        else
                        {
                          forwardHourMotor(1);
                        }
                   }
                     else
                   {
                     if(hourInt > 12 && hourInt <=23)
                     {
                      if(!intialised || hourInt == 15 || hourInt == 18 || hourInt == 21)
                        { 
                         forwardHourMotor(24); // go to the top
                         delay(100);
                         float rotations = 24-hourInt;
                         reverseHourMotor(rotations); //position 24 will be 11pm, postion 23 will be 10pm etc
                         delay(1000);
                        }
                        else
                        {
                          forwardHourMotor(1);
                        }
                     }
                   }
               }
             }// end of else  
             ledBlink(LED_Bottom_HourPin,2);
             ledBlink(LED_Top_HourPin,hourInt);
}


void setMinute()
{
   Serial.print("\nSet Minute:");
   Serial.println(minuteInt); 
         if(minuteInt >= 60 || minuteInt < 0) // should never happen
            {
              reverseMinuteMotor(60); 
              delay(1000);
            }
             else
             {
               if(minuteInt == 0) //60 minutes
               {
                   reverseMinuteMotor(60); //move to bottom switch trigger
                   delay(100);
                   forwardMinuteMotor(0.5); // move to the first position 
               }
               else
               {
                   if(minuteInt <= 30) //1 to 30
                   {
                         
                         if(!intialised || minuteInt == 15 || minuteInt == 30) //do a full reset every 15 min
                         {
                           reverseMinuteMotor(60); // go to the bottom
                           delay(100);
                           float rotations = minuteInt;
                           forwardMinuteMotor(rotations + 1); //position 1 will be 60th minute, postion 2 will be 1 min etc
                           delay(1000);
                         }
                         else
                         {
                          forwardMinuteMotor(1);// just move one minute
                          
                         }
                   }
                     else
                    if(minuteInt > 30 && minuteInt <=59)
                   {
                         
                      if(!intialised || minuteInt == 45) //do a full reset
                         {
                         forwardMinuteMotor(60); // go to the top
                         delay(100);
                         float rotations = 60-minuteInt;
                         reverseMinuteMotor(rotations); //position 60 will be 59 min, postion 59 will be 58 min etc
                         delay(1000);
                         }
                         else
                         {
                          forwardMinuteMotor(1);// just move one minute
                         }
                   }
               }
             }// end of else 
             ledBlink(LED_Bottom_MinutePin,2);   
             ledBlink(LED_Top_MinutePin,minuteInt); 
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
       //for 59 min  
      //int rotationSteps = 2495;
      int rotationSteps = 2700;
      Serial.println("forwarding"); 
       //6400 = 40mm travel
       //3200 = 20mm travel
       //2494 = 15mm travel (1 minute)
       
       long rotationCount = rotationSteps * rotations;
       
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

              digitalWrite(DIR_Minute, HIGH);
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
      Serial.println("reversing");
       //for 59 min  
      int rotationSteps = 2700;
       
       //6400 = 40mm travel
       //3200 = 20mm travel
       //2494 = 15mm travel (1 minute)
       
       long rotationCount = rotationSteps * rotations;

       
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
              digitalWrite(DIR_Minute, LOW);
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
       Serial.println("forwarding...");
       delay(200);
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
       Serial.println("reversing...");
       delay(200);
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
