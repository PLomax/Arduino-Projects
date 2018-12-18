/*

	Stepper motor sketch for Lego Clock
	
	Updated 21/11/2018

*/

const int btTop_A    = 2;
const int btBottom_A = 3;

int buttonStateTopA = 0; 
int buttonStateBottomA = 0; 

int hourInt = 0;
String recd;


//const int btTop_B    = 4;
//const int btBottom_B = 5;

const int PUL_A = 8; //define Pulse pin
const int DIR_A = 7; //define Direction pin
const int ENA_A = 6; //define Enable Pin

//const int PUL_B = 11; //define Pulse pin
//const int DIR_B = 10;//define Direction pin
//const int ENA_B = 9;//define Enable Pin
 
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

 

void setup() {
       Serial.begin(9600);
       pinMode(PUL_A, OUTPUT);
       pinMode(DIR_A, OUTPUT);
       pinMode(ENA_A, OUTPUT);
       pinMode(btTop_A, INPUT);
       pinMode(btBottom_A, INPUT);
       digitalWrite(btTop_A, HIGH); //initialise the button to HIGH
       digitalWrite(btBottom_A, HIGH);//initialise the button to HIGH
}


void loop() {
  delay(500);
  hourInt =0;
  Serial.println(".");
  if (Serial.available() > 0)
  {
      recd = Serial.readString();
      Serial.print("read:");
      if(recd.indexOf('\n') > 0)
         recd =  recd.substring(0,recd.indexOf('\n'));
   
      hourInt = recd.toInt();
      Serial.println(hourInt);
  

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
                     float rotations = 23-hourInt;
                     reverseA(rotations); //position 24 will be 11pm, postion 23 will be 10pm etc
                     delay(1000);
                 
                 
               }
           }
         }
       //reverseA(rotations);
       //delay(1000);
    }
}
