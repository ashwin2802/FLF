#define encoder0PinA  20
#define encoder0PinB  21
#define encoder1PinA  18
#define encoder1PinB  19
#define en0 9
#define en1 5
#define in0PinA 8
#define in0PinB 7
#define in1PinA 4
#define in1PinB 3

volatile long encoder0Pos=0;
volatile long encoder1Pos=0;
long newpositionA;
long newpositionB;
long oldpositionA = 0;
long oldpositionB = 0;
unsigned long newtimeA;
unsigned long newtimeB;
unsigned long oldtimeA = 0;
unsigned long oldtimeB = 0;
long velA;
long velB;

void setup()
{
  pinMode(en0, OUTPUT);
  pinMode(in0PinA, OUTPUT);
  pinMode(in0PinB, OUTPUT);
  pinMode(en1, OUTPUT);
  pinMode(in1PinA, OUTPUT);
  pinMode(in1PinB, OUTPUT);
  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  pinMode(encoder1PinA, INPUT);
  digitalWrite(encoder1PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder1PinB, INPUT);
  digitalWrite(encoder1PinB, HIGH);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoderA, RISING);  // encoDER ON PIN 2
  attachInterrupt(digitalPinToInterrupt(encoder1PinA), doEncoderB, RISING);  // encoDER ON PIN 2
  Serial.begin (9600);
  Serial.println("start");                // a personal quirk
}

void loop()
{
  digitalWrite(in0PinA, LOW);
  digitalWrite(in0PinB, HIGH);
  analogWrite(en0, 250);

  digitalWrite(in1PinA, LOW);
  digitalWrite(in1PinB, HIGH);
  analogWrite(en1, 250);
  
  newpositionA = encoder0Pos;
  newpositionB = encoder1Pos;
  newtimeA = millis();
  newtimeB = millis();
  velA = (newpositionA-oldpositionA) * 1000 /(newtimeA-oldtimeA);
  velB = (newpositionB-oldpositionB) * 1000 /(newtimeB-oldtimeB);
  Serial.print ("speed motor A= ");
  Serial.println(velA);
  Serial.print ("speed motor B= ");
  Serial.println(velB);
  oldpositionA = newpositionA;
  oldpositionB = newpositionB;
  oldtimeA = newtimeA;
  oldtimeB = newtimeB;

  delay(250);
}

void doEncoderA()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
    encoder0Pos++;
  } else {
    encoder0Pos--;
  }
}


void doEncoderB()
{
  if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB)) {
    encoder1Pos++;
  } else {
    encoder1Pos--;
  }
}
