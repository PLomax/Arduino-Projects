/* I2C LCD 16x2 Arduino Tutorial
 * More info http://www.ardumotive.com/i2clcd(en).html
 * Dev: Michalis Vasilakis Date: 19/11/2016 Ver. 1.0 */

//Libraries 
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.


String dateValue;
int timeHour = 1;
int timeMinute = 0;

void setup(){
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)

  dateValue = "11-10-2018";
  
}

void loop()

{
  String hour = String(timeHour);
  String minute = String(timeMinute);
  
  lcd.setCursor(0,0); //First line
  lcd.print(dateValue);
  lcd.setCursor(0,1); //Second line
  
  
  lcd.print(hour+":"+minute);

  timeMinute +=1;
  delay(1000);
}
