#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <ros.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h> 
#include <BfButton.h>
ros::NodeHandle nh;

std_msgs::Int32 pot_msg;
std_msgs::UInt16 pot1_msg;
std_msgs::UInt16 pot2_msg;
ros::Publisher pot1_pub("pot1_value", &pot1_msg);
ros::Publisher pot2_pub("pot2_value", &pot2_msg);

const int potPin1 = A0;
volatile unsigned int temp, counter = 0; 
const int MotorPin10 = 6;  
const int MotorPin11 = 7;  
const int MotorPin6 = 8;  
const int MotorPin7 = 9;  
int LedPin13 = 13; 
int Lswitch = 10;
int speedMM1;
int speedMM2; 
bool rv = false;
bool modeM = false;
bool home_robot = true;
int coun = 0;

int counteren2 = 0;
int angle = 0; 
int aState;
int aLastState;  
int btnPin=13; 
int DT=11; 
int CLK=12; 

int counteren3 = 0;
int angle2 = 0 ;
int aState2;
int aLastState2;  
int DT2=4; 
int CLK2=5;

BfButton btn(BfButton::STANDALONE_DIGITAL, btnPin, true, LOW);
void control_M1(const std_msgs::UInt16& motor_msg) {
 if(modeM){
      if(rv){
    analogWrite(MotorPin7, motor_msg.data);
    digitalWrite(MotorPin6, LOW);
  }
   else{
    if(motor_msg.data <= 0){
      analogWrite(MotorPin7, LOW);
      digitalWrite(MotorPin6, LOW);
    }
    else{
      analogWrite(MotorPin7, -motor_msg.data);
      digitalWrite(MotorPin6, HIGH);
    }
  }
 }
 else{
  
 }
 
}

void control_M2(const std_msgs::UInt16& motor_msg) {
  if(modeM){
      if(rv){
    analogWrite(MotorPin10, motor_msg.data);
    digitalWrite(MotorPin11, LOW);
            }
     else if(!rv) {
          if(motor_msg.data <= 0 ){
              analogWrite(MotorPin10, LOW);
               digitalWrite(MotorPin11, LOW);
              }
           else{
              analogWrite(MotorPin10, -motor_msg.data);
              digitalWrite(MotorPin11, HIGH);
               }
      
              }
         }
          else{
    
    
        }
}

void revers_cb(const std_msgs::Bool& revers_msg) {
  rv = revers_msg.data;
  if (revers_msg.data) {
    
  } 
  else {
  
  }
}
void mode_cb(const std_msgs::Bool& mode_msg) {
  modeM = mode_msg.data;
  if (mode_msg.data) {
    digitalWrite(LedPin13, HIGH);
  } 
  else {
     digitalWrite(LedPin13, LOW);
  }
}
void home_cb(const std_msgs::Bool& home_msg) {
  if(!home_msg.data)coun=0;
   if (coun == 0  ){
    robot_home();
   }
   else{
    
   }
    
    coun = coun+1;
    
}
void pressHandler (BfButton *btn, BfButton::press_pattern_t pattern) {
  switch (pattern) {
    case BfButton::SINGLE_PRESS:
      Serial.println("Single push");
      break;
      
    case BfButton::DOUBLE_PRESS:
      Serial.println("Double push");
      break;
      
    case BfButton::LONG_PRESS:
      Serial.println("Long push");
      break;
  }
}

ros::Subscriber<std_msgs::UInt16> control_Motor1("control_motor1", control_M1);
ros::Subscriber<std_msgs::UInt16> control_Motor2("control_motor2", control_M2);
ros::Subscriber<std_msgs::Bool> revers_sub("revers", revers_cb); 
ros::Subscriber<std_msgs::Bool> mode_sub("mode_state", mode_cb);
ros::Subscriber<std_msgs::Bool> home_sub("home_state", home_cb);
void setup() {
  //Serial.begin(9600);
  robot_home();
  counter=5000;
  counteren3=0;

  pinMode(2, INPUT_PULLUP);  
  pinMode(3, INPUT_PULLUP);

  pinMode(MotorPin10, OUTPUT);
  pinMode(MotorPin11, OUTPUT);
  pinMode(MotorPin6, OUTPUT);
  pinMode(MotorPin7, OUTPUT);
  pinMode(LedPin13, OUTPUT);

  pinMode(Lswitch, INPUT);
  //Serial.begin(9600);
  attachInterrupt(0, ai0, RISING);
  attachInterrupt(1, ai1, RISING);
  pinMode(CLK,INPUT_PULLUP);
  pinMode(DT,INPUT_PULLUP);
  
  pinMode(CLK2,INPUT_PULLUP);
  pinMode(DT2,INPUT_PULLUP);
  
  aLastState = digitalRead(CLK);
  aLastState2 = digitalRead(CLK2);
  
  btn.onPress(pressHandler)
  .onDoublePress(pressHandler) 
  .onPressFor(pressHandler, 1000); 
  
  nh.initNode();
  nh.subscribe(control_Motor1);
  nh.subscribe(control_Motor2);
  nh.subscribe(revers_sub); 
  nh.subscribe(mode_sub);
  nh.subscribe(home_sub); 
  nh.advertise(pot1_pub);
  nh.advertise(pot2_pub);
}
void loop() {
  int potValue1 = analogRead(potPin1);
  //int PositionJoint1 = map(counter, 2600, 5000, -82.5, 82.5);
 // int PositionJoint2 = map(counteren3, 0, 20, -55, 55);
  encoder_ma();
  encoder_RO();
  pot1_msg.data = counter;
  pot1_pub.publish(&pot1_msg);
  pot2_msg.data = counteren3;
  pot2_pub.publish(&pot2_msg);
  speedMM1 = map(potValue1, 0, 1023, 0, 255);
  speedMM2 = counteren2;
 
  if(counter/10 <= 260 ){
    digitalWrite(MotorPin10,LOW );
    digitalWrite(MotorPin11, LOW);
    robot_home();
  }
  if(counteren3 >=20 && rv == false || counteren3 <=0 && rv == true ){
    digitalWrite(MotorPin7,LOW );
    digitalWrite(MotorPin6, LOW);
  }
  else{
    if(!modeM){
    control_MM1();
    control_MM2();
  }
  else{
    
  }
  }
  
  nh.spinOnce();
  delay(1);
}




void ai0() {
  if (digitalRead(3) == LOW) {
    counter++;
    if (counter > 5000) {
      counter = 5000;
    }
  }
    if (counter != temp) {
    counter = constrain(counter, 0, 5000);
    temp = counter;
    if (counter > 5000) {
      counter = 5000;
    }
    if (counter < 1) {
      counter = 1;
    }
  }
}
void ai1() {
  if (digitalRead(2) == LOW) {
    counter--;
    if (counter < 1) {
      counter = 1;
    }
  }
}
void control_MM2() {
  if(rv){
    analogWrite(MotorPin7, speedMM1);
    digitalWrite(MotorPin6, LOW);
  }
  else{
    if(speedMM1 <= 0){
      analogWrite(MotorPin7, LOW);
      digitalWrite(MotorPin6, LOW);
    }
    else{
      analogWrite(MotorPin7, -speedMM1);
      digitalWrite(MotorPin6, HIGH);
    }
  }
}

void control_MM1() {
  if(rv){
    analogWrite(MotorPin10, speedMM2);
    digitalWrite(MotorPin11, LOW);
  }
  //if(digitalRead(btnPin) == LOW && rv == true){
    //analogWrite(MotorPin10, -150);
   //     digitalWrite(MotorPin11, HIGH);
 // }
  else if(!rv) {
    if(speedMM2 <= 0 ){
      analogWrite(MotorPin10, LOW);
      digitalWrite(MotorPin11, LOW);
    }
    else{
        analogWrite(MotorPin10, -speedMM2);
        digitalWrite(MotorPin11, HIGH);
      }
      
  }
 
}
void robot_home(){
  while (digitalRead(Lswitch) == LOW) {
    ai0();
    ai1();
    analogWrite(MotorPin10,170 );
    digitalWrite(MotorPin11, LOW);
    }
    
    while (digitalRead(Lswitch) == HIGH) {
      ai0();
      ai1();
      analogWrite(MotorPin10,-170 );
      digitalWrite(MotorPin11, HIGH);
    }
     digitalWrite(MotorPin10, LOW);
     digitalWrite(MotorPin11, LOW);
     counter=5000;
    
}
void robot_home_2(){
  while (counteren3 > 1) {
     encoder_RO();
     analogWrite(MotorPin7,200 );
     digitalWrite(MotorPin6,LOW);
  }
     digitalWrite(MotorPin6,LOW );
     digitalWrite(MotorPin7, LOW);
     counteren3=0;
    
}
void encoder_ma(){
  btn.read();
  
  aState = digitalRead(CLK);
  if (aState != aLastState){     
     if (digitalRead(DT) != aState) { 
       counteren2 ++;
       angle ++;
     }
     else {
       counteren2--;
       angle --;
     }
     if (counteren2 >=255 ) {
       counteren2 =255;
     }
     if (counteren2 <=0 ) {
       counteren2 =0;
     }
     Serial.println(counteren2); 
  }   
  aLastState = aState;
}
void encoder_RO(){

  
  aState2 = digitalRead(CLK2);
  if (aState2 != aLastState2){     
     if (digitalRead(DT2) != aState2) { 
       counteren3 ++;
       angle2 ++;
     }
     else {
       counteren3--;
       angle2 --;
     }
     if (counteren3 >=20 ) {
       counteren3 =20;
     }
     if (counteren3 <=0 ) {
       counteren3 =0;
     }
     //Serial.println(counteren3); 
  }   
  aLastState2 = aState2;
}
