#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);  // Set the LCD I2C address

void setup()
{
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  
  lcd.home ();  // go home
  lcd.print("Sziasztok !!! ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("Robi vagyok!");      
}

void loop()
{
 // lcd.scrollDisplayLeft();
 // delay(200);
  //-----------------------

 // lcd.noDisplay();
 // delay(200);
 // lcd.display();
  //delay(200);
  //-------------------------
 // lcd.clear();
  //lcd.setCursor(0,1); // column,row starts at 0
  //lcd.setCursor(0,5);
  //lcd.print("HAI..");
  //delay(200);
  
  

}
