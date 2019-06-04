// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

int en = 9;
int in1 = 8;
int in2 = 7;

int pos = 0;
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  
  pinMode(en, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  Serial.println("Encoder Test:");
}

void loop() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(en, 200);

  Serial.print("pin1 = ");
  Serial.println(analogRead("A0"));
  Serial.print("pin2 = ");
  Serial.println(analogRead("A1"));
  
  delay(250);
}
