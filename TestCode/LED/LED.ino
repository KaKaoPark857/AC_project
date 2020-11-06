//LED 테스트 성공
#include <AFMotor.h>
#include <SoftwareSerial.h> //블루투스 시리얼

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  //LED 제어 핀모드 OUTPUT으로 설정
  //아날로그 핀 0~2 LED 1번
  //1번 서랍 LED
  pinMode(A0, OUTPUT); //LED 빨강
  pinMode(A2, OUTPUT); //LED 초록
  pinMode(A1, OUTPUT); //LED 노랑
  //아날로그 핀 3~5 LED 2번
  //2번 서랍 LED
  pinMode(A3, OUTPUT); //LED 빨강
  pinMode(A5, OUTPUT); //LED 초록
  pinMode(A4, OUTPUT); //LED 노랑

}
/*
//1번 LED 점등 제어
void OFF1() {
  Serial.println("ALL LED OFF"); //초기 LED 값
  digitalWrite(A0, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A1, LOW);
}

void RED_ON1(){
  Serial.println("RED ON"); //서랍 잠금 상태(닫힘)
  digitalWrite(A0, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A1, LOW);
}

void GRE_ON1(){
  Serial.println("GREEN ON"); // 서랍 열림 상태
  digitalWrite(A0, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, LOW);
}
void YEL_ON1(){
  Serial.println("YELLOW ON"); //서랍 닫힘 상태
  digitalWrite(A0, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A1, HIGH);
}
//1번 LED 점등 제어 끝

//2번 LED 점등 제어
void OFF2() {
  Serial.println("ALL LED OFF"); //초기 LED 값
  digitalWrite(A3, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A4, LOW);
}

void RED_ON2(){
  Serial.println("RED ON"); //서랍 잠금 상태(닫힘)
  digitalWrite(A3, HIGH);
  digitalWrite(A5, LOW);
  digitalWrite(A4, LOW);
}

void GRE_ON2(){
  Serial.println("GREEN ON"); // 서랍 열림 상태
  digitalWrite(A3, LOW);
  digitalWrite(A5, HIGH);
  digitalWrite(A4, LOW);
}
void YEL_ON2(){
  Serial.println("YELLOW ON"); //서랍 닫힘 상태
  digitalWrite(A3, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A4, HIGH);
}
//2번 LED 점등 제어 끝
*/
 
void loop() {
  // put your main code here, to run repeatedly:
 if(mySerial.available()){
  char in_data;
  in_data = (char)mySerial.read();

  if(in_data=='0') {
    
    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A1, LOW);
  }
  else if(in_data=='1') {
   
  digitalWrite(A0, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A1, LOW);
  }
  else if(in_data=='2') {
      
  digitalWrite(A0, LOW);
  digitalWrite(A2, HIGH);
  digitalWrite(A1, LOW);
  }
  else if(in_data=='3') {
    
  digitalWrite(A0, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A1, HIGH);
  }

  if(in_data=='4') {
    
  digitalWrite(A3, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A4, LOW);
  }
  else if(in_data=='5') {
     
  digitalWrite(A3, HIGH);
  digitalWrite(A5, LOW);
  digitalWrite(A4, LOW);
  }
  else if(in_data=='6') {
    
  digitalWrite(A3, LOW);
  digitalWrite(A5, HIGH);
  digitalWrite(A4, LOW);
  }
  else if(in_data=='7') {

  digitalWrite(A3, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A4, HIGH);
  }
  
/*
  if(in_data=='0') OFF1();
  else if(in_data=='1') RED_ON1();
  else if(in_data=='2') GRE_ON1();
  else if(in_data=='3') YEL_ON1();

  if(in_data=='4') OFF2();
  else if(in_data=='5') RED_ON2();
  else if(in_data=='6') GRE_ON2();
  else if(in_data=='7') YEL_ON2();
*/
 }
 if(Serial.available()){
    mySerial.write(Serial.read());
  }
 
}
