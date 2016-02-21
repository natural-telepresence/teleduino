#include <Servo.h>

/*
   Controls motors
*/

#include <ros.h>
#include <geometry_msgs/Twist.h>

#define LEFT_PIN 6
#define RIGHT_PIN 5

Servo left_motor;
Servo right_motor;

ros::NodeHandle  nh;

void messageCb( const geometry_msgs::Twist& data) {
  digitalWrite(13, HIGH);
  
  float left_duty = data.linear.x + data.angular.z;
  float right_duty = data.linear.x - data.angular.z;
  
  left_duty = add_deadband(max(min(left_duty, 1.0), -1.0));
  right_duty = add_deadband(max(min(right_duty, 1.0), -1.0));

  left_motor.writeMicroseconds(1500 + (int)(left_duty * 500));
  right_motor.writeMicroseconds(1500 + (int)(right_duty * 500));
}

float add_deadband(float value){
  if(value > 0.01){
    return value * 0.4 + 0.05;
  }
  if(value < -0.01){
    return value * 0.4 - 0.05;
  }
  return 0;
}

ros::Subscriber<geometry_msgs::Twist> sub("/base/cmd_vel", &messageCb );

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(13, HIGH);
  left_motor.attach(LEFT_PIN);
  right_motor.attach(RIGHT_PIN);
  left_motor.writeMicroseconds(1500);
  right_motor.writeMicroseconds(1500);
  delay(2000);
  pinMode(13, LOW);
  
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}

