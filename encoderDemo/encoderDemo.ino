// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

int enA = 9;
int inA1 = 7;
int inA2 = 8;
int enB = 5;
int inB1 = 3;
int inB2 = 4;
int encA1 = 18;
int encA2 = 19;
int encB1 = 20;
int encB2 = 21;

int counterA = 0;
int counterB = 0;
int aState;
int aLastState;
int bState;
int bLastState;
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(inA1, OUTPUT);
  pinMode(inA2, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);
  pinMode(encA1, INPUT_PULLUP);
  pinMode(encA2, INPUT_PULLUP);
  pinMode(encB1, INPUT_PULLUP);
  pinMode(encB2, INPUT_PULLUP);
  
  Serial.println("Encoder Test:");
//  aLastState = digitalRead(encA1);
//  bLastState = digitalRead(encB1);
}

void loop() {
  
  digitalWrite(inA1, HIGH);
  digitalWrite(inA2, LOW);
  analogWrite(enA, 250);

  digitalWrite(inB1, LOW);
  digitalWrite(inB2, HIGH);
  analogWrite(enB, 250);
  
  if (aState != aLastState)
  {
    if (digitalRead(encA2) != aState)
    {
      counterA++;
    }
    else
    {
      counterA--;
    }
    Serial.print("Position, Motor 1: ");
    Serial.println(counterA);
  }

  aLastState = aState;

  if (bState != bLastState)
  {
    if (digitalRead(encB2) != bState)
    {
      counterB++;
    }
    else
    {
      counterB--;
    }
    Serial.print("Position, Motor 2: ");
    Serial.println(counterB);
  }
  bLastState = bState;
  
   Serial.print("motor1, pin1 = ");
   Serial.println(digitalRead(encA1));
   Serial.print("motor1, pin2 = ");
   Serial.println(digitalRead(encA2));
   
   Serial.print("motor2, pin1 = ");
   Serial.println(digitalRead(encB1));
   Serial.print("motor2, pin2 = ");
   Serial.println(digitalRead(encB2));
}
