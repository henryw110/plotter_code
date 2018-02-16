

// the maximume in the steper motor is (90degree)
// The maximum in the x direction is (6500)
//The  maximum in the y direction is (7000)





//define each words

#include <Servo.h>
#include <Stepper.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial =  SoftwareSerial(13, 12);
uint8_t buf[9];
#define DRAW 0
#define INITIALISE 1
#define LEFT LOW
#define RIGHT HIGH
#define SPEED 300
//#define Xmax 6500
//#define Ymax 7000
#define POS HIGH
#define NEG LOW
Servo myservo;
int16_t deltaX;
int16_t deltaY;
uint8_t dirX;
uint8_t dirY;
int16_t strCnt;
int16_t endCnt;

int16_t corX;
int16_t corY;

int delta1;
int delta2;
int error;
// defines pins numbers
const int Xlimit_swich = 5;
const int Ylimit_swich = 6;
const int XstepPin = 7; // define pin 7
const int XdirPin = 9; // define pine 9
const int YstepPin = 8; // define pin 8
const int YdirPin = 10; // define pine 10
const int Zservpin = 11; // define pine 11

// initialize each swich

int state = INITIALISE;
int penDown = 90;
int penUp = 0;

void set_x() {
  digitalWrite(XdirPin, LEFT);
  while (digitalRead(Xlimit_swich) == HIGH) {
    digitalWrite(XstepPin, HIGH);
    delayMicroseconds(SPEED);
    digitalWrite(XstepPin, LOW);
    delayMicroseconds(SPEED);
  }
}
void set_y() {
  digitalWrite(YdirPin, RIGHT);
  while (digitalRead(Ylimit_swich) == HIGH) {
    digitalWrite(YstepPin, HIGH);
    delayMicroseconds(SPEED);
    digitalWrite(YstepPin, LOW);
    delayMicroseconds(SPEED);
  }
}


void motor_x(int steps, int dir) {
  digitalWrite(XdirPin, dir);
  for (int x = 0; x < steps; x++) {
    digitalWrite(XstepPin, HIGH);
    delayMicroseconds(SPEED);
    digitalWrite(XstepPin, LOW);
    delayMicroseconds(SPEED);
  }
}
void motor_y(int steps, int dir) {

  digitalWrite(YdirPin, dir);
  for (int x = 0; x < steps; x++) {
    digitalWrite(YstepPin, HIGH);
    delayMicroseconds(SPEED);
    digitalWrite(YstepPin, LOW);
    delayMicroseconds(SPEED);
  }
}



/*void step(boolean dir,int steps){
  digitalWrite(YstepPin, dir);
  digitalWrite(YstepPin, dir);
  delayMicroseconds(SPEED);
  for(int i=0;i<steps;i++){
  digitalWrite(XstepPin, HIGH);
  digitalWrite(YstepPin, HIGH);
  delayMicroseconds(SPEED);
  digitalWrite(XstepPin, LOW);
  digitalWrite(YstepPin, LOW);
  delayMicroseconds(SPEED);
  }

  }*/
// void penUp()

void alg(int x1, int y1)
{
  deltaX = x1;
  deltaY = y1;

  if (deltaX <= 0) dirX = NEG; else dirX = POS;

  if (deltaY <= 0) dirY = POS; else dirY = NEG;

  if (abs(deltaX) >= abs(deltaY)) {
    strCnt = 0;
    endCnt = abs(x1);
    delta1 = deltaX;
    delta2 = deltaY;
  }
  else {
    strCnt = 0;
    endCnt = abs(y1);
    delta1 = deltaY;
    delta2 = deltaX;
  }
  for (int z = strCnt; z < endCnt; z++)
  {
    if (abs(deltaX) >= abs(deltaY)) {
      motor_x(1, dirX);
      corX++;
    }
    else {
      motor_y(1, dirY);
      corY++;
    }
    error = abs(error + abs(delta2));
    if ((2 * error) >= abs(delta1)) {
      error = error - abs(delta1);
      if (abs(deltaX) >= abs(deltaY)) {
        motor_y(1, dirY);
        corY++;
      }
      else {
        motor_x(1, dirX);
        corX++;
      }
    }
    
    //delay(300);
  } 
  mySerial.println("done!");
}
int pos1_x;
int pos2_x;
int pos1_y;
int pos2_y;
void setup()
{
  myservo.write(penDown);
  delay(300);
  myservo.write(penUp);
  delay(300);
  myservo.write(penDown);
  delay(300);
  myservo.write(penUp);
  //while(1);
  mySerial.setTimeout(1000);
  mySerial.begin(9600);
  Serial.begin(9600);
  mySerial.println("live");
  pinMode(XstepPin, OUTPUT);
  pinMode(XdirPin, OUTPUT);
  pinMode(YstepPin, OUTPUT);
  pinMode(YdirPin, OUTPUT);
  myservo.attach(Zservpin);
  // myservo.write(penDown);
  set_x(); // anitilise it to zero
  set_y(); // anitilise it to zero
  mySerial.println("live");
  alg(10000, 350);
}
int delta_x=0;
int delta_y=0;
void loop()
{
  for(int z=0;z++;z<=9)
  {
    buf[z]=0;
  }

  // mySerial.println("live");
  if (Serial.available()==9)
  {
    mySerial.println("Overflow,Stopped");
    while(1);
  }
  while (Serial.available()<=0){;}
  
    int byteLimit = (Serial.readBytes( buf, 5));
    
    pos2_x = (256 * uint8_t(buf[2]) + uint8_t(buf[1]));
    pos2_y = (256 * uint8_t(buf[4]) + uint8_t(buf[3]));
    delta_x = pos2_x - pos1_x;
    delta_y = pos2_y - pos1_y;
    pos1_x = pos2_x;
    pos1_y = pos2_y;
    
  
  if(delta_x!=0&&delta_y!=0)
{
alg(delta_x, delta_y);
//mySerial.println("done"); 
Serial.write(0xFF);

}

// if (buf[0] == 0xFF) {
//    myservo.write(penUp);
//    while (1) {};

 
}
