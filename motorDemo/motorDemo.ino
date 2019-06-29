// Motor A
 
int enA = 9;
int in1 = 7;
int in2 = 8;
int enB = 5;
int inB1 = 3;
int inB2 = 4;
 
void setup()
 
{
 
  // Set all the motor control pins to outputs
 
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(inB1, OUTPUT);
  pinMode(inB2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Motor Test");
 
}

void demoOne()
 
{
 
  // This function will run the motors in both directions at a fixed speed
 
  // Turn on motor A
 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);
 
  // Set speed to 200 out of possible range 0~255
 
  analogWrite(enA, 250);
  analogWrite(enB, 250);
 
  //delay(2000);
 
  // Now change motor directions
 
  //digitalWrite(in1, LOW);
  //digitalWrite(in2, HIGH);  
  //digitalWrite(inB1, HIGH);
  //digitalWrite(inB2, LOW);  
 
  //delay(2000);
 
  // Now turn off motors
 
  //digitalWrite(in1, LOW);
  //digitalWrite(in2, LOW); 
    //digitalWrite(inB1, LOW);
  //digitalWrite(inB2, LOW);
   

}
 
void demoTwo()
 
{
 
  // This function will run the motors across the range of possible speeds
  // Note that maximum speed is determined by the motor itself and the operating voltage
 
  // Turn on motors
 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
    digitalWrite(inB1, HIGH);
  digitalWrite(inB2, LOW);  

  // Accelerate from zero to maximum speed
 
  for (int i = 0; i < 256; i++)
 
  {
 
    analogWrite(enA, i);
    analogWrite(enB, i);
    
    delay(20);
 
  } 
 
  // Decelerate from maximum speed to zero
 
  for (int i = 255; i >= 0; --i)
 
  {
 
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
 
  } 
 
  // Now turn off motors
 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);   
   
  digitalWrite(inB1, LOW);
  digitalWrite(inB2, LOW); 
}
 
void loop()
 
{
 
  demoOne();
 
  delay(1000);
 
  //demoTwo();
 
  //delay(1000);
 
}
