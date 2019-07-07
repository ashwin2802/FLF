#include <SoftwareSerial.h>
#define minBlackVal 650
#define minFrontBlackVal 700

#define rightMotor1 3
#define led 13
#define rightMotor2 4
#define rightMotorEn 5
#define leftMotor1 7
#define leftMotor2 8
#define leftMotorEn 9
#define len 3

SoftwareSerial BTserial(10,11);
//unsigned int values[64] = {0, 5001, 4001, 4502, 2501, 4003, 3502, 4007, 2501, 3504, 3003, 4510, 2502, 2510, 3007, 3513, 1001, 3005, 2504, 4511, 2003, 3008, 3510, 4014, 1502, 1511, 1510, 3018, 2007, 2014, 2513, 3019, 1, 2506, 2005, 4512, 1504, 4009, 3511, 4015, 1003, 1009, 2008, 4516, 2510, 2516, 3014, 3520, 502, 512, 511, 2517, 510, 516, 2018, 4021, 1007, 1015, 1014, 3021, 1513, 1520, 2019, 2522};
unsigned int values[64] = {0, 5001, 4001, 4502, 2501, 4012, 3502, 4003, 2501, 3522, 3012, 4513, 2502, 2513, 3003, 3504, 1001, 2532, 2522, 4523, 2012, 4033, 3513, 4014, 1502, 1523, 1513, 3024, 2003, 2014, 2504, 2505, 1, 2542, 2032, 4543, 1522, 4053, 3523, 4034, 1012, 1053, 2033, 3044, 2513, 2554, 3014, 2515, 502, 543, 513, 2564, 513, 2044, 2024, 2515, 1003, 1024, 1014, 2515, 1504, 2515, 2505, 2506};
// kP = -1.260001167, kD = -0.06000057, kI = 0
// for rightBaseSpeed = 140 and leftBaseSpeed = 120
float gains[len] = {-1.260001167,-0.06000057,0};
float dgains[len] = {1,1,1};
float tol = 0.002, step_ = 0.005;

float rightMaxSpeed = 250;
float leftMaxSpeed = 215;
float rightBaseSpeed = 250;
float leftBaseSpeed = 215;
float rightTurnSpeed = 200;
float leftTurnSpeed = 170;

//float maxSpeed = 250;
//float baseSpeed = 250;

int backPos = 0, backType = 0, backValue = 0;
int frontDelta = 0, backDelta = 0;
int frontValue = 0, frontType = 0, frontPos = 0;
float frontDeltaN = 0, backDeltaN = 0;
float error = 0, lastError = 0, bestError = 0, sumError = 0;
float rightMotorSpeed = 0, leftMotorSpeed = 0;
float setpt = 2500.0;
int count = 0;
//float motorSpeed = 0;

int power(int b)
{
  if(b==0) return 1;
  if(b==1) return 2;
  if(b==2) return 4;
  if(b==3) return 8;
  if(b==4) return 16;
  if(b==5) return 32; 
}

int readBackLine(){
  
  unsigned int sensors[6];
  int reading = 0; 
  
  sensors[0] = analogRead(A6);
  sensors[1] = analogRead(A7);
  sensors[2] = analogRead(A8);
  sensors[3] = analogRead(A9);
  sensors[4] = analogRead(A10);
  sensors[5] = analogRead(A11);

  for(int i=0; i<6; i++)
  {
    int bit_;
    if(sensors[i] > minBlackVal)
      bit_ = 1;
    else bit_ = 0;
    reading += bit_ * power(i);  
  }
  count = count + 1;

//  if(count % 20 == 0)
//  { 
//    count = 0;

//  BTserial.println("back");
//  BTserial.print(sensors[5]);
//  BTserial.print((sensors[5]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
  
//  BTserial.print(sensors[4]);
//  BTserial.print((sensors[4]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
 
//  BTserial.print(sensors[3]);
//  BTserial.print((sensors[3]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
  
//  BTserial.print(sensors[2]);
//  BTserial.print((sensors[2]>minBlackVal)?"B":"W");
//  BTserial.print("\t");

//  BTserial.print(sensors[1]);
//  BTserial.print((sensors[1]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
    
//  BTserial.print(sensors[0]);
//  BTserial.print((sensors[0]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
  
//  BTserial.println(";");
//  }
  
//  BTserial.print(reading);
//  BTserial.println(";");

  return values[reading];
}

int readFrontLine(){
  
  unsigned int sensors[6];
  int reading = 0; 
  
  sensors[0] = analogRead(A0);
  sensors[1] = analogRead(A1);
  sensors[2] = analogRead(A2);
  sensors[3] = analogRead(A3);
  sensors[4] = analogRead(A4);
  sensors[5] = analogRead(A5);

  for(int i=0; i<6; i++)
  {
    int bit_;
    if(sensors[i] > minFrontBlackVal)
      bit_ = 1;
    else bit_ = 0;
    reading += bit_ * power(i);  
  }
  count = count + 1;

//  if(count % 20 == 0)
//  { 
//    count = 0;

//  BTserial.println("front");
//  BTserial.print(sensors[5]);
//  BTserial.print((sensors[5]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
  
//  BTserial.print(sensors[4]);
//  BTserial.print((sensors[4]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
 
//  BTserial.print(sensors[3]);
//  BTserial.print((sensors[3]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
  
//  BTserial.print(sensors[2]);
//  BTserial.print((sensors[2]>minBlackVal)?"B":"W");
//  BTserial.print("\t");

//  BTserial.print(sensors[1]);
//  BTserial.print((sensors[1]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
    
//  BTserial.print(sensors[0]);
//  BTserial.print((sensors[0]>minBlackVal)?"B":"W");
//  BTserial.print("\t");
  
//  BTserial.println(";");
//  }

//  BTserial.print(reading);
//  BTserial.println(";");

  return values[reading];
}

void leftTurn()
{
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorEn, rightBaseSpeed);

    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    analogWrite(leftMotorEn, leftBaseSpeed);
    return;
}

float run()
{
  frontValue = readFrontLine();
  backValue = readBackLine();

  frontType = frontValue %100;
  backType = backValue % 100;

  frontPos = frontValue - frontType;
  backPos = backValue - backType;

  frontDelta = (frontPos - setpt);
  backDelta = (backPos - setpt);
  backDeltaN = backDelta / setpt;
  
  error = (gains[0]*abs(backDeltaN) + gains[1]*(abs(backDeltaN)-lastError) + gains[2]*sumError);
  sumError += abs(backDeltaN);
  lastError = abs(backDeltaN);
//  motorSpeed = error*baseSpeed;

  if(backDelta < 0){
    if(abs(frontDelta) > 1400 && abs(frontDelta) < 2100)
    {
      rightMotorSpeed = rightTurnSpeed;
      leftMotorSpeed = leftTurnSpeed - error*leftTurnSpeed;
    }
    else
    {
      rightMotorSpeed = rightBaseSpeed;
      leftMotorSpeed = leftBaseSpeed - error*leftBaseSpeed;  
    }
  }
  else if(backDelta == 0){
    rightMotorSpeed = rightBaseSpeed;
    leftMotorSpeed = leftBaseSpeed;
  }
  else if(backDelta > 0){
    if(abs(frontDelta) > 1400 && abs(frontDelta) < 2100){
      leftMotorSpeed = leftTurnSpeed;
      rightMotorSpeed = rightTurnSpeed - error*rightTurnSpeed;
    }
    else{
      leftMotorSpeed = leftBaseSpeed;
      rightMotorSpeed = rightBaseSpeed - error*rightBaseSpeed;
    }
  }
  
//  rightMotorSpeed = rightBaseSpeed + motorSpeed;
//  leftMotorSpeed = leftBaseSpeed - motorSpeed;

  if(rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed;
  if(leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed;
  if(rightMotorSpeed < 0) rightMotorSpeed = 0;
  if(leftMotorSpeed < 0) leftMotorSpeed = 0;

//  if(count % 20 == 0){
//    count = 0;
//    BTserial.print(error);
//    BTserial.print("\t");
//    BTserial.print(rightMotorSpeed);  
//    BTserial.print("\t");
//    BTserial.print(leftMotorSpeed);
//    BTserial.println(";");
//  }

//   if(frontValue == 0 && (backValue == 2502 || backValue == 3001 || backValue == 2001))
//   {
//    do{
////      leftTurn(); 
//    }while(backValue != 2502 && backValue != 3001 && backValue != 2001);
//   }
//   if(backValue == 2522)
//   {
//      do
//      {
////        leftTurn(); 
//        backValue = readBackLine();
//      }while(backValue != 2502 && backValue != 3001 && backValue != 2001);
//   }
//   else
//   {

    if(backType % 10 > 4)
    {
        digitalWrite(rightMotor1, HIGH);
        digitalWrite(rightMotor2, LOW);
        analogWrite(rightMotorEn, rightMotorSpeed);
  
        digitalWrite(leftMotor1, HIGH);
        digitalWrite(leftMotor2, LOW);
        analogWrite(leftMotorEn, leftMotorSpeed);
    }
    else{ 
      if(backDelta == 0){
        digitalWrite(rightMotor1, HIGH);
        digitalWrite(rightMotor2, LOW);
        analogWrite(rightMotorEn, rightMotorSpeed);
  
        digitalWrite(leftMotor1, HIGH);
        digitalWrite(leftMotor2, LOW);
        analogWrite(leftMotorEn, leftMotorSpeed);
      }
      else if(backDelta < 0){
        digitalWrite(rightMotor1, HIGH);
        digitalWrite(rightMotor2, LOW);
        analogWrite(rightMotorEn, rightMotorSpeed);
  
        digitalWrite(leftMotor1, LOW);
        digitalWrite(leftMotor2, HIGH);
        analogWrite(leftMotorEn, leftMotorSpeed);
      }
      else if(backDelta > 0){
        digitalWrite(rightMotor1, LOW);
        digitalWrite(rightMotor2, HIGH);
        analogWrite(rightMotorEn, rightMotorSpeed);
  
        digitalWrite(leftMotor1, HIGH);
        digitalWrite(leftMotor2, LOW);
        analogWrite(leftMotorEn, leftMotorSpeed);
      }
    }
//  }

  return error;
}

void tune(){
  dgains[0] = dgains[1] = 1;
  dgains[2] = 0;
  bestError = abs(run());
  while(dgains[0]+dgains[1]+dgains[2]>tol){
    for(int i=0; i<len; i++){
      gains[i] = gains[i] + dgains[i];
      error = abs(run());
      if(error < bestError){
        bestError = error;
        dgains[i] = dgains[i] * (1 + step_);
      }
      else{
        gains[i] = gains[i] - 2*dgains[i];
        error = abs(run());
        if(error < bestError){
          bestError = error;
          dgains[i] = dgains[i] * (1 + step_);
        }
        else{
          gains[i] = gains[i] + dgains[i];
          dgains[i] = dgains[i] * (1 - step_);
        }
      }
    }
      BTserial.print(gains[0]);
      BTserial.print("\t\t");
      BTserial.print(gains[1]);
      BTserial.print("\t\t");
      BTserial.print(gains[2]);
      BTserial.println("\t\t");
      BTserial.print(dgains[0]);
      BTserial.print("\t\t");
      BTserial.print(dgains[1]);
      BTserial.print("\t\t");
      BTserial.print(dgains[2]);
      BTserial.println(";");
  }
}

void setup() {
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorEn, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorEn, OUTPUT);
  pinMode(led, OUTPUT);

  BTserial.begin(9600);
  delay(5000);
  BTserial.println("tuning start");
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
//  tune();
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  
  BTserial.println("tuning over");
  BTserial.print(gains[0], 8);  
  BTserial.print("\t\t\t\t");
  BTserial.print(gains[1], 8);
  BTserial.print("\t\t\t\t");
  BTserial.print(gains[2], 8);
  BTserial.print("\t\t\t\t");
  BTserial.print(bestError, 20);
  BTserial.println(";");
  
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  analogWrite(rightMotorEn, 0);

  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  analogWrite(leftMotorEn, 0);
  
//  delay(10000);
}

void loop() {
  error = run(); 
//  BTserial.print(error);
//  BTserial.println(";"); 
}
