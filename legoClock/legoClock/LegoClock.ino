/*
      Lego Clock
      Created 22/11/2018
      
      Last updated 13/01/2019
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
const int LED_Top_Hour= 30;
const int LED_Bottom_Hour= 26;
const int LED_Top_Minute= 31;
const int LED_Bottom_Minute= 27;

//Sound
const int hourSoundPin = 5;

bool initialisedTime = false;

void setup() {
  Serial.begin(9600);
  Serial.println("starting..");
  esp8266Module.begin(57600);
  

  Serial.println("lego clock..");
  

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

   //LED Settings
   pinMode(LED_Top_Hour, OUTPUT);
   pinMode(LED_Bottom_Hour, OUTPUT);
   pinMode(LED_Top_Minute, OUTPUT);
   pinMode(LED_Bottom_Minute, OUTPUT);
   
   //turn on LEDs
   digitalWrite(LED_Top_Hour, HIGH);
   digitalWrite(LED_Bottom_Hour, HIGH);
   digitalWrite(LED_Top_Minute, HIGH);
   digitalWrite(LED_Bottom_Minute, HIGH);
   
   //Sound Settings
   pinMode(hourSoundPin , OUTPUT);

   ledBlink(LED_Top_Minute,5);
   ledBlink(LED_Bottom_Minute,5);
   delay(1000);
   ledBlink(LED_Top_Hour,5);
   ledBlink(LED_Bottom_Hour,5);
  
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
      
      String timeString =  dateTimeVal.substring(index+1);
      Serial.println(timeString);
      
      int colonIndex = timeString.indexOf(':');
      if(colonIndex > 0)
      {

        hourStr = timeString.substring(0,colonIndex);
        minuteStr = timeString.substring(colonIndex+1,colonIndex+3);
        Serial.println(hourStr);
        Serial.println(minuteStr);
        lastCheckCount =0;
        lastCheckStr = String(lastCheckCount); 
        
        if(minuteInt != minuteStr.toInt())
        {
            int oldMinuteValue = minuteInt;
            minuteInt = minuteStr.toInt();
            setMinute(oldMinuteValue,minuteInt);

            //implement minute here
        }         
        
        if(hourInt != hourStr.toInt()) // hour has updated
        {
            int oldHourValue = hourInt;
            hourInt = hourStr.toInt();
            setHour(oldHourValue,hourInt);
            
        }// end of hour test
      }

      initialisedTime = true;
    }
    
    
   }

 
  lastCheckStr = String(lastCheckCount); 
  delay(1000);


}


void ledBlink(int pinID, int blinkCount)
{
  Serial.println(""); 
  for(int i=0;i < blinkCount; i++)
  {
    digitalWrite(pinID, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                       // wait for a second
    digitalWrite(pinID, LOW);    // turn the LED off by making the voltage LOW
    delay(100);    
    Serial.print("*");
  }
  Serial.println("");
  digitalWrite(pinID, HIGH);
}

void hourChime(int count)
{
    if(count > 12)
      count = count -12;
  
    for(int i=0;i<count;i++)
    {
    digitalWrite(hourSoundPin, HIGH);
    delay(1000);
    digitalWrite(hourSoundPin, LOW);
    delay(1000);    
    }
 }

void setHour(int oldHourValue, int newHourValue)
{
      Serial.print("\nSet Hour:");
      Serial.println(newHourValue);

      ledBlink(LED_Top_Hour,3);
      
      if(newHourValue >= 24 || newHourValue < 0) // should never happen
            {
              reverseHourMotor(24); 
              delay(1000);
            }
            else
             {
               if(newHourValue == 0) //midnight
               {
                   reverseHourMotor(24); //move to bottom switch trigger
                   delay(100);
                   forwardHourMotor(1); // move to the first position 
               }
               else
               {
                   if(initialisedTime)
                   {
                     forwardHourMotor(newHourValue - oldHourValue); //advance time by 1 hour
                     delay(1000);                   
                    
                   }

                   else
                   {
                     reverseHourMotor(24); // go to the bottom
                     delay(100);
                     float rotations = newHourValue;
                     forwardHourMotor(rotations + 1); //position 1 will be midnight, postion 2 will be 1am etc
                     delay(1000);
                   }
               }
             }// end of else  

            hourChime(newHourValue);
}


void setMinute(int oldMinuteValue,int newMinuteValue)
{
   ledBlink(LED_Top_Minute, 3);
   
   Serial.print("\nSet Minute:");
   Serial.println(minuteInt); 
         if(newMinuteValue >= 60 || newMinuteValue < 0) // should never happen
            {
              reverseMinuteMotor(60); 
              delay(1000);
            }
             else
             {
          if(newMinuteValue == 0) //60 minutes
               {
                   reverseMinuteMotor(60); //move to bottom switch trigger
                   delay(100);
                   forwardMinuteMotor(0.5); // move to the first position 
               }
               else
               {
                 if(initialisedTime)
                 {
                     forwardMinuteMotor(newMinuteValue - oldMinuteValue); //advance minutes by difference
                     delay(1000);                  
                 }

                 else
                 {
                     reverseMinuteMotor(60); // go to the bottom
                     delay(100);
                     float rotations = newMinuteValue;
                     forwardMinuteMotor(rotations + 1); //position 1 will be 60th minute, postion 2 will be 1 min etc
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
