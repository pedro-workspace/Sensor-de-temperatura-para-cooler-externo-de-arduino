// C++ code
//
int temp = 0;

int i = 0;

int motor = 0;

int buzzer = 0;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop()
{
  delay(1000); // Wait for 1000 millisecond(s)
  temp = (-40 + 0.488155 * (analogRead(A0) - 20));
  if (temp >= 30) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
  delay(1000); // Wait for 1000 millisecond(s)
  if (temp >= 50) {
    tone(4, 523, 1000); // play tone 60 (C5 = 523 Hz)
  } else {
    noTone(4);
  }
}