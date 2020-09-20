#include <Arduino.h>
#include <NewPing.h>

#define US_FRONT_PING 40
#define US_FRONT_ECHO 41
#define US_REAR_PING 42
#define US_REAR_ECHO 43
#define US_LEFT_PING 44
#define US_LEFT_ECHO 45
#define US_RIGHT_PING 46
#define US_RIGHT_ECHO 47
#define MAX_DIST 500

NewPing us_front(US_FRONT_PING, US_FRONT_ECHO, MAX_DIST);
NewPing us_rear(US_REAR_PING, US_REAR_ECHO, MAX_DIST);
NewPing us_left(US_LEFT_PING, US_LEFT_ECHO, MAX_DIST);
NewPing us_right(US_RIGHT_PING, US_RIGHT_ECHO, MAX_DIST);

//Motor Connections
#define FR_ENA 4
#define FR_IN1 25
#define FR_IN2 24

#define FL_ENB 3
#define FL_IN3 23
#define FL_IN4 22

#define RR_ENA 6
#define RR_IN1 29
#define RR_IN2 28

#define RL_ENB 5
#define RL_IN3 27
#define RL_IN4 26

void setup()
{
  Serial.begin(9600);
  // All motor control pins are outputs
  pinMode(FR_ENA, OUTPUT);
  pinMode(FR_IN1, OUTPUT);
  pinMode(FR_IN2, OUTPUT);

  pinMode(FL_ENB, OUTPUT);
  pinMode(FL_IN3, OUTPUT);
  pinMode(FL_IN4, OUTPUT);

  pinMode(RR_ENA, OUTPUT);
  pinMode(RR_IN1, OUTPUT);
  pinMode(RR_IN2, OUTPUT);

  pinMode(RL_ENB, OUTPUT);
  pinMode(RL_IN3, OUTPUT);
  pinMode(RL_IN4, OUTPUT);
}
void goForward(int speed)
{
  // turn on front motors
  digitalWrite(FR_IN1, HIGH);
  digitalWrite(FR_IN2, LOW);
  digitalWrite(FL_IN3, LOW);
  digitalWrite(FL_IN4, HIGH);
  //turn on rear motors
  digitalWrite(RR_IN1, HIGH);
  digitalWrite(RR_IN2, LOW);
  digitalWrite(RL_IN3, LOW);
  digitalWrite(RL_IN4, HIGH);
  // set speed
  analogWrite(FR_ENA, speed);
  analogWrite(FL_ENB, speed);
  analogWrite(RR_ENA, speed);
  analogWrite(RL_ENB, speed);
  //delay(time);
}
void goBackwards(int time)
{
  // turn on front motors
  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, HIGH);
  digitalWrite(FL_IN3, HIGH);
  digitalWrite(FL_IN4, LOW);
  //turn on rear motors
  digitalWrite(RR_IN1, LOW);
  digitalWrite(RR_IN2, HIGH);
  digitalWrite(RL_IN3, HIGH);
  digitalWrite(RL_IN4, LOW);
  // set speed
  int speed = 255;
  analogWrite(FR_ENA, speed);
  analogWrite(FL_ENB, speed);
  analogWrite(RR_ENA, speed);
  analogWrite(RL_ENB, speed);

  delay(time);
}
void turnRight(int speed, int turningTime)
{
  //left wheels go forward
  digitalWrite(FL_IN3, LOW);
  digitalWrite(FL_IN4, HIGH);
  digitalWrite(RL_IN3, LOW);
  digitalWrite(RL_IN4, HIGH);
  //right wheels go backwards
  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, HIGH);
  digitalWrite(RR_IN1, LOW);
  digitalWrite(RR_IN2, HIGH);
  // set speed
  analogWrite(FR_ENA, speed);
  analogWrite(FL_ENB, speed);
  analogWrite(RR_ENA, speed);
  analogWrite(RL_ENB, speed);
  //this is the time it takes to execute a 90 degree turn
  delay(turningTime);
}
void turnLeft(int speed, int turningTime)
{
  //left wheels go backwards
  digitalWrite(FL_IN3, HIGH);
  digitalWrite(FL_IN4, LOW);
  digitalWrite(RL_IN3, HIGH);
  digitalWrite(RL_IN4, LOW);
  //right wheels go forwards
  digitalWrite(FR_IN1, HIGH);
  digitalWrite(FR_IN2, LOW);
  digitalWrite(RR_IN1, HIGH);
  digitalWrite(RR_IN2, LOW);
  // set speed
  analogWrite(FR_ENA, speed);
  analogWrite(FL_ENB, speed);
  analogWrite(RR_ENA, speed);
  analogWrite(RL_ENB, speed);
  //delay for the time it takes to execute a 90 degree turn
  delay(turningTime);
}

void brake()
{
  digitalWrite(FR_IN1, LOW);
  digitalWrite(FR_IN2, LOW);
  digitalWrite(FL_IN3, LOW);
  digitalWrite(FL_IN4, LOW);
  //turn on rear motors
  digitalWrite(RR_IN1, LOW);
  digitalWrite(RR_IN2, LOW);
  digitalWrite(RL_IN3, LOW);
  digitalWrite(RL_IN4, LOW);

  // delay(time);
}
float k = 343.0 * pow(10, -4) * 0.5; //constant to convert milliseconds to cm
void us_measure()
{
  Serial.print(us_front.ping_median(3) * k);
  Serial.print(",");
  //Serial.print(us_rear.ping_median(3) * k);
  ////Serial.print(",");
  //// Serial.print(us_left.ping_median(3) * k);
  //Serial.print(",");
  Serial.print(us_right.ping_median(3) * k);
 
}
void measurement_run()
{
  goForward(165);
  int count = 0;
  while (count < 9)
  {
    us_measure();
    delay(1000);
    count++;
  }
  brake();
}
int count = 0;
void loop()
{
  
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();
    if (cmd == 's')
    {
      //measurement_run();
      goForward(255);
      us_measure();
      delay(50);
      brake();
      count++;
    }
    if (count >= 38)
    {
      brake();
      delay(5000);
    }
  }
}
