#include <SoftwareSerial.h>
#define minBlackVal 500

#define rightMotor1 3
#define rightMotor2 4
#define rightMotorEn 5
#define leftMotor1 7
#define leftMotor2 8
#define leftMotorEn 9
#define len 2

SoftwareSerial BTserial(10,11);
unsigned int values[64] = {0, 5001, 4001, 4502, 3001, 4003, 3502, 4007, 2001, 3504, 3003, 4510, 2502, 2510, 3007, 3513, 1001, 3005, 2504, 4511, 2003, 3008, 3510, 4014, 1502, 1511, 1510, 3018, 2007, 2014, 2513, 3019, 1, 2506, 2005, 4512, 1504, 4009, 3511, 4015, 1003, 1009, 2008, 4516, 2510, 2516, 3014, 3520, 502, 512, 511, 2517, 510, 516, 2018, 4021, 1007, 1015, 1014, 3021, 1513, 1520, 2019, 2522};
float gains[len] = {0,0};
float dgains[len] = {1,1};
float tol = 0.002;

float rightMaxSpeed = 250;
float leftMaxSpeed = 250;
float rightBaseSpeed = 150;
float leftBaseSpeed = 150;

int pos_ = 0, type = 0, value = 0;
float error = 0, lastError = 0, bestError = 0;
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

  if(reading != 0 && count % 100 == 0)
  { 
    count = 0;
//  BTserial.print(sensors[5]);
  BTserial.println("back");
  BTserial.print((sensors[5]>minBlackVal)?"B":"W");
  BTserial.print("\t");
  
//  BTserial.print(sensors[4]);
  BTserial.print((sensors[4]>minBlackVal)?"B":"W");
  BTserial.print("\t");
 
//  BTserial.print(sensors[3]);
  BTserial.print((sensors[3]>minBlackVal)?"B":"W");
  BTserial.print("\t");
  
//  BTserial.print(sensors[2]);
  BTserial.print((sensors[2]>minBlackVal)?"B":"W");
  BTserial.print("\t");

//  BTserial.print(sensors[1]);
  BTserial.print((sensors[1]>minBlackVal)?"B":"W");
  BTserial.print("\t");
    
//  BTserial.print(sensors[0]);
  BTserial.print((sensors[0]>minBlackVal)?"B":"W");
  BTserial.print("\t");
  
  BTserial.print(";");
  }
//  BTserial.print(reading);
//  BTserial.print(";");

  return values[reading];
}

float run()
{
  float Kp = gains[0];
  float Kd = gains[1];

  value = readBackLine();
  type = value % 100;

  pos_ = value - type;
  error = ((pos_ - 2500.0)*rightBaseSpeed)/2500.0;
  motorSpeed = Kp*error + Kd*(error-lastError);
  lastError = error;

  rightMotorSpeed = rightBaseSpeed - motorSpeed;
  leftMotorSpeed = leftBaseSpeed + motorSpeed;

  if(rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed;
  if(leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed;
  if(rightMotorSpeed < 0) rightMotorSpeed = 0;
  if(leftMotorSpeed < 0) leftMotorSpeed = 0;

  if(count %100 == 0){
    count = 0;
    BTserial.print(rightMotorSpeed);  
    BTserial.print("\t");
    BTserial.print(leftMotorSpeed);
    BTserial.print(";");
  }

  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  analogWrite(rightMotorEn, rightMotorSpeed);

  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  analogWrite(leftMotorEn, leftMotorSpeed);

  return error;
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
          dgains[i] = dgains[i] * 1.1;
        }
        else{
          gains[i] = gains[i] + dgains[i];
          dgains[i] = dgains[i] * 0.9;
        }
      }
    }
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

  BTserial.begin(9600);
  
  tune();
  
  BTserial.print(gains[0]);  
  BTserial.print("\t");
  BTserial.print(gains[1]);
  BTserial.print(";");
  BTserial.print(bestError);
  BTserial.print(";");
}

void loop() {
  // put your main code here, to run repeatedly:
  error = run();
  BTserial.print(error);
  BTserial.print(";");
}
