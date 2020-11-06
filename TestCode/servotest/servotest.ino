//서보테스트 성공
//오랜지색 pwm / 빨간색 vcc(5v) / 갈색 ground
#include <Servo.h>
#include <AFMotor.h>
#include <SoftwareSerial.h> //블루투스 시리얼


SoftwareSerial mySerial(2,3); // RX, TX

Servo myser1;
Servo myser2;
int value1 = 90;
int value2 = 90;
//+30 왼쪽 30도 / -30 오른쪽 30도


void setup(){
  Serial.begin(9600);
  mySerial.begin(9600);
  myser1.attach(10);
  myser2.attach(9);
}

void loop(){
  if(mySerial.available()){
    char in_data;
    in_data = (char)mySerial.read();
   if(in_data=='1'){ //lock 오른쪽으로 각도
    Serial.println("1lock");
     value1 -= 30;
     myser1.write(value1);
   } 
   else if(in_data=='2'){ //unlock 왼쪽으로 각도
    Serial.println("1unlock");
     value1 += 30;
     myser1.write(value1);
   }
   else if(in_data=='3') {
    Serial.println("2lock");
    value2 -= 30; 
    myser2.write(value2);
   }
   else if(in_data=='4') {
    Serial.println("2unlock");
    value2 += 30; 
    myser2.write(value2);    
   }
  }
  if(Serial.available()){
    mySerial.write(Serial.read());
  }
}
