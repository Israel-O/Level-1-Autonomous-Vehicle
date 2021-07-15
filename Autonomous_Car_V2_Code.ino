//Servo
#include <Servo.h>
Servo myServo;

int tolerance = 6;
int breakTime = 2000;
int reverseTime = 400;

//L293D_1 MOTOR 1&2
const int motorPin1 = 2;
const int motorPin2 = 3;

const int motorPin3 = 4;
const int motorPin4 = 7;

//L293D_2 MOTOR 3&4
const int motorPin5 = 8;
const int motorPin6 = 12;

const int motorPin7 = 13;
const int motorPin8 = 10;

//Ultrasonic Sensor
const int trigPin = 28;
const int echoPin = 26;
int distance;
int duration;

//Best Position mechanism
int dir = 0;
int servoSpeed = 10;
int check = 0;
int quadrant = 0;
int localDistance = 0;
int stopDistance = 5;
int gDist = 0;

void setup() {
  // put your setup code here, to run once:

  //Start serial comms
  Serial.begin(9600);
  
  //Motor pinmode points
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin6, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);

  //Ultrasonic Sensor 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Servo
  myServo.attach(6);
  myServo.write(90);
}

void forward(){
  //Back Left wheel
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  //Back Right Wheel 
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  //Front Left Wheel 
  digitalWrite(motorPin5, HIGH);
  digitalWrite(motorPin6, LOW);
  //Front Right Wheel 
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, HIGH);

  Serial.println("Forward");
}

void reverse(){
  //Back Left wheel 
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  //Back Right Wheel 
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  //Front Left Wheel 
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, HIGH);
  //Front Right Wheel 
  digitalWrite(motorPin7, HIGH);
  digitalWrite(motorPin8, LOW);

  Serial.println("reverse");
}

void left(){
  //Back Left wheel (I think) changed-return both to low
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  //Back Right Wheel 
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  //Front Left Wheel 
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, HIGH);
  //Front Right Wheel
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, HIGH);

  Serial.println("left");
}

void right(){
  //Back Left wheel 
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  //Back Right Wheel 
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  //Front Left Wheel 
  digitalWrite(motorPin5, HIGH);
  digitalWrite(motorPin6, LOW);
  //Front Right Wheel 
  digitalWrite(motorPin7, HIGH);
  digitalWrite(motorPin8, LOW);

  Serial.println("right");
}

void dontMove(){
  //Back Left wheel 
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  //Back Right Wheel 
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  //Front Left Wheel 
  digitalWrite(motorPin5, LOW);
  digitalWrite(motorPin6, LOW);
  //Front Right Wheel 
  digitalWrite(motorPin7, LOW);
  digitalWrite(motorPin8, LOW);

  Serial.println("dontMove");
}

void loop() {
  // put your main code here, to run repeatedly:
  localDistance = readDistance();
  if (localDistance <= stopDistance && localDistance != 0){
    dontMove();
    delay(breakTime);
    dir = servoSweep(); //Use an LED to see how long this lasts for
    if (dir < 0){  //Left quadrant [designated by a negative max distance]
      reverse();
      delay(reverseTime);
      left();
      delay(1000);
      dontMove();
      delay(breakTime);
    }
  
    else {  //Right quadrant [designated by a positive max distance]
      reverse();
      delay(reverseTime);
      right();
      delay(1000);
      dontMove();
      delay(breakTime);
    }
  }
  else{
    forward();
  }
  
}

int readDistance(){
  digitalWrite(trigPin, LOW);
  delay(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);

  distance = (0.034 / 2) * duration;
  return distance;
}

int servoSweep(){
  quadrant = 1;
  int i;
  int j;
  int k;
  
  for (i = 90; i >= 0; i--){ //servo turns right
    myServo.write(i);
    delay(servoSpeed);
  }

  for (j = 0; j < 90; j++){  //back to centre
    check = readDistance(); //read distance
    if (check >= gDist){
      gDist = check;
      quadrant = 1;
    }
    myServo.write(j);
    delay(servoSpeed);
  }
  
  for (int k = 90; j <= 180; j++){ //to the left
    check = readDistance(); //read distance
    if (check >= gDist){
      gDist = check;
      quadrant = -1;      
    }
    myServo.write(j);
    delay(servoSpeed);
  } 
  
  for (int i = 180; i >= 90; i--){ //back to centre
    myServo.write(i);
    delay(servoSpeed);
  }
  return quadrant;
  
}
