#include<SoftwareSerial.h>
SoftwareSerial BTserial(10,11); // RX | TX
int a,b,c,d,e,f;
char A,B,C,D,E,F; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTserial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  a=analogRead(A5);
  b=analogRead(A4);
  c=analogRead(A3);
  d=analogRead(A2);
  e=analogRead(A1);
  f=analogRead(A0);

  if(a>500) A='B';
  else A='W';
  if(b>500) B='B';
  else B='W';
  if(c>500) C='B';
  else C='W';
  if(d>500) D='B';
  else D='W';
  if(e>500) E='B';
  else E='W';
  if(f>500) F='B';
  else F='W';
  Serial.print(A);
  Serial.println(B);
  Serial.write(C);
  Serial.write(D);
  Serial.write(E);
  Serial.write(F);
  Serial.println();
  BTserial.write(A);
  BTserial.write(B);
  BTserial.write(C);
  BTserial.write(D);
  BTserial.write(E);
  BTserial.write(F);
    BTserial.print(";");
    delay(500);

}
