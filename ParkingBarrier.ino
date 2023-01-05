// Parking sensor > Barrier & Light control
// 

//---> start of code

#include <Servo.h>

Servo servo_barrier;

//constant
const int servo_pin = 2;
const int sensor_echo = 3;
const int sensor_trig = 4;
const int LedRed = 6;
const int LedGreen = 5;
const int ServoMinAngle = 0;
const int ServoMaxAngle = 90;
const int ServoAngleStep = 15; //degrees
const int MaxDistanceDetect = 40; //in inches

//variables
float duration;
float distance;
int angle = 0;
int currentBarrierState;


void setup() 
{
  Serial.begin(9600);
  servo_barrier.attach(servo_pin);
  servo_barrier.write(ServoMinAngle);
  currentBarrierState = servo_barrier.read();
  Serial.println("Barrier at initial position.");
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(sensor_trig, OUTPUT);
  pinMode(sensor_echo, INPUT);
  digitalWrite(LedRed, HIGH);
  Serial.println("Setup/Initialize Complete!");
}


void loop() 
{
  Serial.println("Start Loop!");
  currentBarrierState = servo_barrier.read();
  Serial.println("currentBarrierState");
  Serial.println(currentBarrierState);

  
  //Output 1 ms pulse
  digitalWrite(sensor_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_trig, HIGH);  
  delayMicroseconds(10);
  digitalWrite(sensor_trig, LOW);
  
  //Measure the pulse
  duration = pulseIn(sensor_echo, HIGH);
  distance = ((duration / 2.00) / 29.10) / 2.54; //to get distance in 1 direction (in cm / inches)
  Serial.println("Duration: ");
  Serial.println(distance);

  
  
  if (distance <= MaxDistanceDetect && distance >=0) 
  {
    if (currentBarrierState == 0)
    {
      for (angle = ServoMinAngle; angle <= ServoMaxAngle; angle += ServoAngleStep)
      {
        servo_barrier.write(angle);
        delay(15);     
      }
      digitalWrite(LedRed, LOW);
      delay (1000);
      digitalWrite(LedGreen, HIGH);
      delay (5000);
    }
    else
    {
      //do nothing, object still within the detectable range
    }
  }
  else
  {
    if (currentBarrierState != 0)
    {
      for (angle = ServoMaxAngle; angle >= ServoMinAngle; angle -= ServoAngleStep)
      {
        servo_barrier.write(angle);
        delay(15);
      } 
      digitalWrite(LedGreen, LOW);
      delay (1000);
      digitalWrite(LedRed, HIGH);
      delay (5000);
    }
    else
    {
      //do nothing, barrier is down
    }
  }

}
  
 
