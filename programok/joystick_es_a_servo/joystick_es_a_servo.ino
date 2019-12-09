/* 37. rész - A joystick modul
 * 
 * Készítette: Kotán Tamás Balázs - Magyar Arduino Labor
 * 
 * Győr, 2017.08.03.
 */


#include <Servo.h>

Servo szervo1;
Servo szervo2;

int X_koord = A0;
int Y_koord = A1;

int Joystick_val;

void setup() {
  // put your setup code here, to run once:

szervo1.attach(5);
szervo2.attach(6);

Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

Joystick_val = analogRead(X_koord);
Joystick_val = map (Joystick_val, 0, 1023, 0, 180);
szervo1.write(Joystick_val);
Serial.print("X: ");
Serial.println(Joystick_val);


Joystick_val = analogRead(Y_koord);
Joystick_val = map (Joystick_val, 0, 1023, 0, 180);
szervo2.write(Joystick_val);
Serial.print("Y: ");
Serial.println(Joystick_val);

Serial.println();
delay(15);

}
