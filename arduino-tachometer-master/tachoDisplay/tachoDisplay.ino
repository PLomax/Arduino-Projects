/*
 * SIMPLE ARDUINO CAR TACHOMETER
 */
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.


const int DATA_PIN            = 4;
const int LED_GROUND_PIN      = 8;

const int PINS_COUNT          = 4;
const int LED_PINS[PINS_COUNT]        = {9,    10,   11,   12};
const int LED_SWITCH_RPM[PINS_COUNT]  = {1600, 2000, 3000, 4000};
const int REV_LIMITER_RPM             = 5800;

const int NUMBER_OF_CYLINDERS = 4;
const int LED_UPDATE_INTERVAL = 200;

/*
 * Last led state update time in ms, used to calculate the time from last update
 */
unsigned long lastUpdateTime  = 0;

/*
 * Amount of spark fires in a single interval
 */
int sparkFireCount            = 0;

/*
 * Rpm value from last update
 * Used to average the last 2 rpms for smoother output
 */
int lastRpmValue              = 0;

/*
 * Blinking rev limiter state
 */
bool revLimiterOn             = false;

/*
 *
 */
void incrementRpmCount () {
  sparkFireCount++;
}

/*
 * Turns all leds on or off
 */
void setGlobalState(bool state) {
  for (int i = 0; i < PINS_COUNT; i++) {
    digitalWrite(LED_PINS[i], state);
  }
}

/*
 * Turn on leds, based on input rpm
 */
void setLedState(int rpm) {
  setGlobalState(LOW);

  // If rpm is over REV_LIMITER_RPM, all leds should be blinking at 200ms interval
  if (rpm > REV_LIMITER_RPM) {
    if (revLimiterOn) {
      setGlobalState(LOW);
    } else {
      setGlobalState(HIGH);
    }

    revLimiterOn = !revLimiterOn;
    return;
  }

  for (int i = 0; i < PINS_COUNT; i++) {
      if (rpm > LED_SWITCH_RPM[i]) {
          digitalWrite(LED_PINS[i], HIGH);
      }
  }
}

/*
 * Defines led pins as output,
 * turns all leds on for 500ms when started
 * attach to serial if available
 */
void setup() {
     lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
    lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
    lcd.setCursor(0,0); //First line
    lcd.print("RPM:");
  
  // Define all led pins as outputs
  for (int i = 0; i < PINS_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  pinMode(LED_GROUND_PIN, OUTPUT);
  digitalWrite(LED_GROUND_PIN, LOW); // Use pin 8 as ground for the leds

  setGlobalState(HIGH);
  delay(500);
  setGlobalState(LOW);

  pinMode(DATA_PIN, INPUT_PULLUP);
  attachInterrupt(1, incrementRpmCount, FALLING);
  Serial.begin(9600);
}

// 4 stroke engine fires every spark in 2 revolutions
// so calculate at what degree interval sparks fires and divide 360 by it,
// to find the number of fires per rotation
const int FIRES_PER_REV = (360 / (720 / NUMBER_OF_CYLINDERS));

void loop() {
//  if ((millis() - lastUpdateTime) > LED_UPDATE_INTERVAL) {

    // multiply the amount the spark fires in one interval by the number of intervals per
    // second, to find the amount in one second
    // then multiply the amount in one second by 60, to find the spark fires in one minute and
    // divide the result by the number of fires per revolution to find the rpm
    int currentRpm = (sparkFireCount * (1000 / LED_UPDATE_INTERVAL) * 60) / FIRES_PER_REV;
    currentRpm = currentRpm/3.5;

    // average the current and last rpm for smoother results
    int averagedRpm = (currentRpm + lastRpmValue) / 2;

    setLedState(averagedRpm);
    Serial.println(averagedRpm);

    setDisplay(averagedRpm);

    sparkFireCount = 0;
  //  lastUpdateTime = millis();
    lastRpmValue = currentRpm;
    delay(1000);
//  }
}

void setDisplay(int value)
{
    lcd.setCursor(4,0); //First line
    lcd.print("        ");
    lcd.setCursor(4,0);
    lcd.print(value);

    lcd.setCursor(0,1); // line 2
    lcd.print("          ");
    lcd.setCursor(0,1);
    if(value > 0 && value < 500)
    {
      lcd.print("="); //1
    }
    else
    if(value >500 && value < 1000)
       {
      lcd.print("=="); //2
    } 
    else
    if(value >1000 && value < 1500)
       {
      lcd.print("===");//3
    }     
      else
    if(value >1500 && value < 1000)
       {
      lcd.print("====");//4
    } 
        else
    if(value >2000 && value < 2500)
       {
      lcd.print("=====");//5
    } 
        else
    if(value >2500 && value < 3000)
       {
      lcd.print("======");//6
    } 
        else
    if(value >3000 && value < 3500)
       {
      lcd.print("=======");//7
    } 
        else
    if(value >3500 && value < 4000)
       {
      lcd.print("========");//8
    } 

        else
    if(value >4000 && value < 4500)
       {
      lcd.print("=========");//9
    } 
        else
    if(value >4500 && value < 5000)
       {
      lcd.print("==========");//10
    } 

        else
    if(value >5000 && value < 5500)
       {
      lcd.print("===========");//11
    } 
      
        else
    if(value >5500 && value < 6000)
       {
      lcd.print("============"); //12
    } 

        else
    if(value >6000 && value < 6500)
       {
      lcd.print("============="); //13
    } 
            else
    if(value >6500 && value < 7000)
       {
      lcd.print("=============="); //14
    } 
            else
    if(value >7000 && value < 7500)
       {
      lcd.print("===============");//15
    } 
            else
    if(value >7500 && value < 8000)
       {
      lcd.print("================");//16
    } 


}
