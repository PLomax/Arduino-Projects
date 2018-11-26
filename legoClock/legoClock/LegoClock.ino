/*
      Lego Clock
      Created 22/11/2018
      
      Last updated 22/11/2018
  */


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


//Stepper Motor Control

//threashold buttons
const int btTop_A    = 2;
const int btBottom_A = 3;

//const int btTop_B    = 4;
//const int btBottom_B = 5;

int buttonStateTopA = 0; 
int buttonStateBottomA = 0; 

//Stepper connections
const int PUL_A = 8; //define Pulse pin
const int DIR_A = 7; //define Direction pin
const int ENA_A = 6; //define Enable Pin

//const int PUL_B = 11; //define Pulse pin
//const int DIR_B = 10;//define Direction pin
//const int ENA_B = 9;//define Enable Pin

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

  //Stepper settings
   pinMode(PUL_A, OUTPUT);
   pinMode(DIR_A, OUTPUT);
   pinMode(ENA_A, OUTPUT);
   pinMode(btTop_A, INPUT);
   pinMode(btBottom_A, INPUT);
   digitalWrite(btTop_A, HIGH); //initialise the button to HIGH
   digitalWrite(btBottom_A, HIGH);//initialise the button to HIGH
  
  
  
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
        hourInt = hourStr.toInt();
        minuteInt = minuteStr.toInt();
        
        //move the motors
        
        if(hourInt >= 24 || hourInt < 0) // should never happen
        {
          reverseA(24); 
          delay(1000);

        }
         else
         {
           if(hourInt == 0) //midnight
           {
               reverseA(24); //move to bottom switch trigger
               delay(100);
               forwardA(1); // move to the first position 
           }
           else
           {
               if(hourInt < 13)
               {
                     reverseA(24); // go to the bottom
                     delay(100);
                     float rotations = hourInt;
                     forwardA(rotations + 1); //position 1 will be midnight, postion 2 will be 1am etc
                     delay(1000);
               }
                 else
               {
                     forwardA(24); // go to the top
                     delay(100);
                     float rotations = hourInt;
                     reverseA(rotations - 1); //position 24 will be 11pm, postion 23 will be 10pm etc
                     delay(1000);
                 
                 
               }
           }
         }
        
      }
    }
    
    
   }

 
  lcd.setCursor(6,1); //Second line
  lastCheckStr = String(lastCheckCount); 
  lcd.print("wibble:"+lastCheckStr);
  delay(1000);


}


void releaseMotorA()
{
    delay(100);
    digitalWrite(DIR_A,LOW);
    digitalWrite(ENA_A,LOW);
    digitalWrite(PUL_A,LOW);
    delay(100);
}

void forwardA(float rotations)
{
       long rotationCount = 6400 * rotations;
       
       buttonStateTopA = digitalRead(btTop_A);
       Serial.print("\nForward:\ntopState:");
       Serial.println(buttonStateTopA);
       for (long i = 0; i<rotationCount; i++)    //Forward 5000 steps
       {
              if (buttonStateTopA == HIGH)//TOP switch triggered exit function
              {
                    Serial.print("TOP switch triggered exit function");
                    break; 
              }

              digitalWrite(DIR_A, LOW);
              digitalWrite(ENA_A, HIGH);
              digitalWrite(PUL_A, HIGH);
              delayMicroseconds(50);
              digitalWrite(PUL_A, LOW);
              delayMicroseconds(50);

              buttonStateTopA = digitalRead(btTop_A);
       }
       
       releaseMotorA();
       
}


 

void reverseA(float rotations)
{
       long rotationCount = 6400 * rotations;
       
       buttonStateBottomA = digitalRead(btBottom_A);
       Serial.print("\nReverse:\nbottomState:");
       Serial.println(buttonStateBottomA);
       
       for (long i = 0; i < rotationCount; i++)   //Backward 5000 steps
       {
              if (buttonStateBottomA == HIGH) //BOTTOM switch triggered exit function
              {
                    Serial.print("BOTTOM switch triggered exit function");
                    break;  
              }
              digitalWrite(DIR_A, HIGH);
              digitalWrite(ENA_A, HIGH);
              digitalWrite(PUL_A, HIGH);
              delayMicroseconds(60);
              digitalWrite(PUL_A, LOW);
              delayMicroseconds(60);
              
              buttonStateBottomA = digitalRead(btBottom_A);
       }

      releaseMotorA();
}


