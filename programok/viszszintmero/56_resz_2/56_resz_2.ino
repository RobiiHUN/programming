/*
  55.rész - Vízszintmérés Arduinoval

  Készítette: Kotán Tamás Balázs - Magyar Arduino Labor

  2018.03.26.
*/

const int sensor = A0;
int level;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  level = analogRead(sensor);
  Serial.println(level);

  if (level >= 0 && level < 550)
  {
    Serial.println("A vizszint 25% alatt");
  }
  else if (level > 549 && level < 580)
  {
    Serial.println("A vizszint 25%-50% között");
  }

  else if (level > 579 && level < 600)
  {
    Serial.println("A vizszint 50%-75% között");
  }

  else if (level > 600)
  {
    Serial.println("A vizszint 75%-100% között");
  }
  else
  {
    Serial.println("HIBA!");
  }
  delay(500);

}


