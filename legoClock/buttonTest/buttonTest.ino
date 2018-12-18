
const int btTop_Hour    = 11;
const int btBottom_Hour = 10;

const int btTop_Minute    = 9;
const int btBottom_Minute = 8;

int btStateTopHour = 0; 
int btStateBottomHour = 0; 

int btStateTopMinute = 0; 
int btStateBottomMinute = 0; 

void setup() {
  Serial.begin(9600);
  Serial.println("starting..");

  
   pinMode(btTop_Hour, INPUT);
   pinMode(btBottom_Hour, INPUT);
   digitalWrite(btTop_Hour, HIGH); //initialise the button to HIGH
   digitalWrite(btBottom_Hour, HIGH);//initialise the button to HIGH


   pinMode(btTop_Minute, INPUT);
   pinMode(btBottom_Minute, INPUT);
   digitalWrite(btTop_Minute, HIGH); //initialise the button to HIGH
   digitalWrite(btBottom_Minute, HIGH);//initialise the button to HIGH
}

void loop() {

       delay(200);
       btStateTopHour = digitalRead(btTop_Hour);
       Serial.print("\nbtStateTopHour:");
       Serial.println(btStateTopHour);
      if (btStateTopHour == HIGH)//TOP switch triggered exit function
      {
            Serial.print("TOP Hour switch triggered exit function");
            delay(3000);
      }
      
       btStateBottomHour = digitalRead(btBottom_Hour);
       Serial.print("\nbtStateBottomHour:");
       Serial.println(btStateBottomHour);
      if (btStateBottomHour == HIGH)//Bottom switch triggered exit function
      {
            Serial.println("Bottom Hour switch triggered exit function");
            delay(3000);
      }      
      
       btStateTopMinute = digitalRead(btTop_Minute);
       Serial.print("\nbtStateTopMinute:");
       Serial.println(btStateTopMinute);
      if (btStateTopMinute == HIGH)//TOP switch triggered exit function
      {
            Serial.println("TOP min switch triggered exit function");
            delay(3000);
      }      
      
       btStateBottomMinute = digitalRead(btBottom_Minute);
       Serial.print("\nbtStateTopMinute:");
       Serial.println(btStateBottomMinute);
      if (btStateBottomMinute == HIGH)//TOP switch triggered exit function
      {
            Serial.println("Bottom min switch triggered exit function");
            delay(3000);
      }      
      
      
      
      // btStateTopMinute = digitalRead(btTop_Minute);
       
    
}
