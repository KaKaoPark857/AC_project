//오랜지색 pwm / 빨간색 vcc(5v) / 갈색 ground
#include <Servo.h>
#include <AFMotor.h>

Servo myser1;
Servo myser2;
int value1 = 90;
int value2 = 90;
//+30 왼쪽 30도 / -30 오른쪽 30도
void setup(){
  Serial.begin(9600);
  myser1.attach(10);
  myser2.attach(9);
}

void loop(){
  if(Serial.available()){
    char in_data;
    in_data = Serial.read();
   if(in_data=='1'){ //lock 오른쪽으로 각도
     value1 -= 60;
     myser1.write(value1);
   } 
   else if(in_data=='2'){ //unlock 왼쪽으로 각도
     value1 += 60;
     myser1.write(value1);
   }
   else if(in_data=='3') {
    value2 -= 60; 
    myser2.write(value2);
   }
   else if(in_data=='4') {
    value2 += 60; 
    myser2.write(value2);    
   }
  }
}
