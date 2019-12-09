const int ledpin=13; // ledpin and soundpin are not changed throughout the process 
const int soundpin=A2;
const int threshold=200; // sets threshold value for sound sensor
void setup() {
Serial.begin(9600);
pinMode(ledpin,OUTPUT);
pinMode(soundpin,INPUT);
}
void loop() {
int soundsens=analogRead(soundpin); // reads analog data from sound sensor
if (soundsens>=threshold) {
digitalWrite(ledpin,HIGH); //turns led on
delay(1000);
}
else{
digitalWrite(ledpin,LOW);
}
}
