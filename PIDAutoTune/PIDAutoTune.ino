#include <SoftwareSerial.h>
#define minBlackVal 500

#define rightMotor1 3
#define led 13
#define rightMotor2 4
#define rightMotorEn 5
#define leftMotor1 7
#define leftMotor2 8
#define leftMotorEn 9
#define len 3

SoftwareSerial BTserial(10,11);
unsigned int values[64] = {0, 5001, 4001, 4502, 3001, 4003, 3502, 4007, 2001, 3504, 3003, 4510, 2502, 2510, 3007, 3513, 1001, 3005, 2504, 4511, 2003, 3008, 3510, 4014, 1502, 1511, 1510, 3018, 2007, 2014, 2513, 3019, 1, 2506, 2005, 4512, 1504, 4009, 3511, 4015, 1003, 1009, 2008, 4516, 2510, 2516, 3014, 3520, 502, 512, 511, 2517, 510, 516, 2018, 4021, 1007, 1015, 1014, 3021, 1513, 1520, 2019, 2522};
float gains[len] = {0,0,0};
float dgains[len] = {1,1,1};
float tol = 0.002, step_ = 0.1;

//float rightMaxSpeed = 250;
//float leftMaxSpeed = 250;
//float rightBaseSpeed = 80;
//float leftBaseSpeed = 80;

float maxSpeed = 250;
float baseSpeed = 80;

int pos_ = 0, type = 0, value = 0;
float error = 0, lastError = 0, bestError = 0, sumError = 0;
float motorSpeed = 0;
float rightMotorSpeed = 0, leftMotorSpeed = 0;

int count = 0;

int power(int a, int b)
{
  if(b==0)
    return 1;
    if(b==1)
    return 2;
    if(b==2)
    return 4;
    if(b==3)
    return 8;
    if(b==4)
    return 16;
    if(b==5)
    return 32; 
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
    reading += bit_ * power(2, i);  
  }
  count = count + 1;

  if(count % 20 == 0)
  { 
    count = 0;
//  Serial.print(sensors[5]);
  BTserial.println("back");
  BTserial.print((sensors[5]>minBlackVal)?"B":"W");
  BTserial.print("\t");
  
//  Serial.print(sensors[4]);
  BTserial.print((sensors[4]>minBlackVal)?"B":"W");
  BTserial.print("\t");
 
//  Serial.print(sensors[3]);
  BTserial.print((sensors[3]>minBlackVal)?"B":"W");
  BTserial.print("\t");
  
//  Serial.print(sensors[2]);
  BTserial.print((sensors[2]>minBlackVal)?"B":"W");
  BTserial.print("\t");

//  Serial.print(sensors[1]);
  BTserial.print((sensors[1]>minBlackVal)?"B":"W");
  BTserial.print("\t");
    
//  Serial.print(sensors[0]);
  BTserial.print((sensors[0]>minBlackVal)?"B":"W");
  BTserial.print("\t");
  
  BTserial.println(";");
  }
//  Serial.print(reading);
//  Serial.print(";");

  return values[reading];
}

float run()
{
  value = readBackLine();
  type = value % 100;

  pos_ = value - type;
  error = (pos_ - 2500.0)/2500.0;
  motorSpeed = (gains[0]*abs(error) + gains[1]*(abs(error)-lastError) + gains[2]*sumError)*baseSpeed;
  sumError += abs(error);
  lastError = abs(error);

  if(error < 0){
    rightMotorSpeed = baseSpeed;
    leftMotorSpeed = motorSpeed;
  }
  else if(error == 0){
    rightMotorSpeed = leftMotorSpeed = baseSpeed;
  }
  else{
    rightMotorSpeed = baseSpeed;
    leftMotorSpeed = motorSpeed;
  }
  
//  rightMotorSpeed = rightBaseSpeed + motorSpeed;
//  leftMotorSpeed = leftBaseSpeed - motorSpeed;

  if(rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = maxSpeed;
  if(leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = maxSpeed;
  if(rightMotorSpeed < 0) rightMotorSpeed = 0;
  if(leftMotorSpeed < 0) leftMotorSpeed = 0;

//  if(count % 20 == 0){
    count = 0;
    BTserial.print(error);
    BTserial.print("\t");
    BTserial.print(rightMotorSpeed);  
    BTserial.print("\t");
    BTserial.print(leftMotorSpeed);
    BTserial.println(";");
//  }

  if(error == 0){
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorEn, rightMotorSpeed);

    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite(leftMotorEn, leftMotorSpeed);
  }
  else if(error < 0){
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorEn, rightMotorSpeed);

    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    analogWrite(leftMotorEn, leftMotorSpeed);
  }
  else{
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
    analogWrite(rightMotorEn, rightMotorSpeed);

    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite(leftMotorEn, leftMotorSpeed);
  }

  return motorSpeed;
}

void tune(){
  gains[0] = gains[1] = 0;
  dgains[0] = dgains[1] = 1;
  bestError = run();
  while(dgains[0]+dgains[1]>tol){
    for(int i=0; i<len; i++){
      gains[i] = gains[i] + dgains[i];
      error = run();
      if(error < bestError){
        bestError = error;
        dgains[i] = dgains[i] * 1.1;
      }
      else{
        gains[i] = gains[i] - 2*dgains[i];
        error = run();
        if(error < bestError){
          bestError = error;
          dgains[i] = dgains[i] * (1 + step_);
        }
        else{
          gains[i] = gains[i] + dgains[i];
          dgains[i] = dgains[i] * (1 - step_);;
        }
      }
    }
      BTserial.print(gains[0]);
      BTserial.print("\t");
      BTserial.print(gains[1]);
      BTserial.print("\t");
      BTserial.print(gains[2]);
      BTserial.println("\t");
      BTserial.print(dgains[0]);
      BTserial.print("\t");
      BTserial.print(dgains[1]);
      BTserial.print("\t");
      BTserial.print(dgains[2]);
      BTserial.println(";");
  }
}

void setup() {
  // put your setup code here, to run once:
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
  tune();
  digitalWrite(led, LOW);
  BTserial.println("tuning over");
  BTserial.print(gains[0]);  
  BTserial.print("\t");
  BTserial.print(gains[1]);
  BTserial.print("\t");
  BTserial.print(bestError);
  BTserial.println(";");
}

void loop() {
  // put your main code here, to run repeatedly:
  error = run();
  if(count % 20 == 1){
    BTserial.print(error);
    BTserial.println(";");
  }
}
