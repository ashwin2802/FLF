#include <SoftwareSerial.h>
#define minBlackVal 500

#define rightMaxSpeed 250
#define leftMaxSpeed 250
#define rightBaseSpeed 150
#define leftBaseSpeed 150

#define rightMotor1 3
#define rightMotor2 4
#define rightMotorEn 5
#define leftMotor1 7
#define leftMotor2 8
#define leftMotorEn 9

SoftwareSerial BTserial(10,11);
unsigned int values[64] = {0, 5001, 4001, 4502, 3001, 4003, 3502, 4007, 2001, 3504, 3003, 4510, 2502, 2510, 3007, 3513, 1001, 3005, 2504, 4511, 2003, 3008, 3510, 4014, 1502, 1511, 1510, 3018, 2007, 2014, 2513, 3019, 1, 2506, 2005, 4512, 1504, 4009, 3511, 4015, 1003, 1009, 2008, 4516, 2510, 2516, 3014, 3520, 502, 512, 511, 2517, 510, 516, 2018, 4021, 1007, 1015, 1014, 3021, 1513, 1520, 2019, 2522};
float Kp = 1;
float Kd = 1;

void setup() {
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorEn, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorEn, OUTPUT);

  BTserial.begin(9600);
} 

int position = 0, type = 0, value = 0;
int error = 0, lastError = 0;
int motorSpeed = 0;
int rightMotorSpeed = 0, leftMotorSpeed = 0;

void loop() {
  value = readLine();
  type = value % 100;

  if(type == 1 or type==2){
    position = value - type;
    error = position - 2500;
    motorSpeed = Kp*error + Kd*(error-lastError);
    lastError = error;

    rightMotorSpeed = rightBaseSpeed + motorSpeed;
    leftMotorSpeed = leftBaseSpeed - motorSpeed;

    if(rightMotorSpeed > rightMaxSpeed) rightMotorSpeed = rightMaxSpeed;
    if(leftMotorSpeed > leftMaxSpeed) leftMotorSpeed = leftMaxSpeed;
    if(rightMotorSpeed < 0) rightMotorSpeed = 0;
    if(leftMotorSpeed < 0) leftMotorSpeed = 0;

    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    digitalWrite(rightMotorEn, rightMotorSpeed);

    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(leftMotorEn, leftMotorSpeed);
  }
}

int readLine(){
  unsigned int sensors[6];
  int reading = 0; 
  sensors[0] = analogRead(A0);
  sensors[1] = analogRead(A1);
  sensors[2] = analogRead(A2);
  sensors[3] = analogRead(A3);
  sensors[4] = analogRead(A4);
  sensors[5] = analogRead(A5);

  for(int i=0; i<6; i++){
    bool bit_ = (sensors[i] > minBlackVal);
    BTserial.write(bit_);
    BTserial.write("\t");
    reading += bit_ * pow(2, 5-i);
  }

  BTserial.print(";");
  delay(500);
  return values[reading];
}
