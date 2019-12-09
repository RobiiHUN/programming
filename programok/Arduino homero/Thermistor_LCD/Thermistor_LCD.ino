#include<math.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

double Thermister(int data)
{
  double temp;
  temp=log(10000.0*((1024.0/data-1)));
  temp=1/(0.001129148+(0.000234125+(0.0000000876741*temp*temp))*temp);
  temp=temp-273.15;
  Serial.println(" ");
  Serial.print(temp);
  Serial.print(" Celcius");
  lcd.setCursor(0,0);
  lcd.print("Temp = ");
  lcd.print(temp);
  lcd.print(" C");
  temp=(temp*9.0)/5.0+32.0;
  Serial.println(" ");
   Serial.print(temp);
  Serial.print(" Fahrenheit");
  Serial.println(" ");
  Serial.println("..................................");
  lcd.setCursor(0,1);
  lcd.print("Temp = ");
  lcd.print(temp);
  lcd.print(" F");
  }

void setup() 
{
Serial.begin(9600);
lcd.begin();
lcd.backlight();
}
int i;
void loop() 
{
i=analogRead(A0);
Thermister(i);
delay(1000);
}
