const int btTop_A    = 2;
const int btBottom_A = 3;

int buttonStateTopA = 0; 
int buttonStateBottomA = 0; 


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
       int rotationCount = 6400 * rotations;
       buttonStateTopA = digitalRead(btTop_A);
       
       for (int i = 0; i<rotationCount; i++)    //Forward 5000 steps
       {
              if (buttonStateTopA == HIGH)//TOP switch triggered exit function
              {
                    releaseMotorA();
                    return; 
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
       int rotationCount = 6400 * rotations;
       buttonStateBottomA = digitalRead(btBottom_A);
       
       for (int i = 0; i < rotationCount; i++)   //Backward 5000 steps
       {
              if (buttonStateBottomA == HIGH) //BOTTOM switch triggered exit function
              {
                    releaseMotorA();
                    return; 
              }
              digitalWrite(DIR_A, HIGH);
              digitalWrite(ENA_A, HIGH);
              digitalWrite(PUL_A, HIGH);
              delayMicroseconds(50);
              digitalWrite(PUL_A, LOW);
              delayMicroseconds(50);
              
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

       Serial.println("loop");
       float rotations = 1.5;
       forwardA(rotations);
       delay(1000);
       reverseA(rotations);
       delay(1000);

}
