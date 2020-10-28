#include <Servo.h>


//1번 서랍 LED 빨강 4번 / 노랑 3번 / 초록 2번
#define r1 4
#define y1 3
#define g1 2
//2번 서랍 LED 빨강 7번 / 노랑 6번 / 초록 5번
#define r2 7
#define y2 6
#define g2 5

#define motor1 12 //모터 1 방향 제어 핀 설정
#define motor2 13 //모터 2 방향 제어 핀 설정
#define speed1 10 //모터 1 속도 제어 핀 설정
#define speed2 11 //모터2 속도 제어 핀 설정

#define servo1 8 //1번 서보모터
#define servo2 9 //2번 서보모터
/*
int motor1 = 12; 
int speed1 = 10; 
int motor2 = 13;
int speed2 = 11;
*/
Servo lock_device1; //서보모터 1
Servo lock_device2; //서보모터 2

//int servo1 = 8;
int value1 = 0; //1번 서보모터 초기각도(0도)
//int servo2 = 9;
int value2 = 0; //2번 서보모터 초기각도(0도)

void setup(){
  Serial.begin(9600); //시리얼 통신 설정
 
 //모터슬라이더 핀모드를 OUTPUT으로 설정 (1번 모터 10,12 / 2번 모터 11, 13)
  pinMode(motor1, OUTPUT); //1번 방향
  pinMode(motor2, OUTPUT); //2번 방향
  pinMode(speed1, OUTPUT); //1번 속도
  pinMode(speed2, OUTPUT); //2번 속도

  //서보모터 핀모드 OUTPUT으로 설정 (1번 서보 8 / 2번 서보 9)
  lock_device1.attach(servo1);  // 1번 서보모터
  lock_device2.attach(servo2);  // 2번 서보모터

  //LED 제어 핀모드 OUTPUT으로 설정
  //1번 서랍 LED
  pinMode(r1, OUTPUT); //LED 빨강
  pinMode(g1, OUTPUT); //LED 초록
  pinMode(y1, OUTPUT); //LED 노랑
  //2번 서랍 LED
  pinMode(r2, OUTPUT); //LED 빨강
  pinMode(g2, OUTPUT); //LED 초록
  pinMode(y2, OUTPUT); //LED 노랑
  
}

//1번 LED 점등 제어
void OFF1() {
  Serial.println("ALL LED OFF"); //초기 LED 값
  digitalWrite(r1, LOW);
  digitalWrite(g1, LOW);
  digitalWrite(y1, LOW);
}

void RED_ON1(){
  Serial.println("RED ON"); //서랍 잠금 상태(닫힘)
  digitalWrite(r1, HIGH);
  digitalWrite(g1, LOW);
  digitalWrite(y1, LOW);
}

void GRE_ON1(){
  Serial.println("GREEN ON"); // 서랍 열림 상태
  digitalWrite(r1, LOW);
  digitalWrite(g1, HIGH);
  digitalWrite(y1, LOW);
}
void YEL_ON1(){
  Serial.println("YELLOW ON"); //서랍 닫힘 상태
  digitalWrite(r1, LOW);
  digitalWrite(g1, LOW);
  digitalWrite(y1, HIGH);
}
//1번 LED 점등 제어 끝

//2번 LED 점등 제어
void OFF2() {
  Serial.println("ALL LED OFF"); //초기 LED 값
  digitalWrite(r2, LOW);
  digitalWrite(g2, LOW);
  digitalWrite(y2, LOW);
}

void RED_ON2(){
  Serial.println("RED ON"); //서랍 잠금 상태(닫힘)
  digitalWrite(r2, HIGH);
  digitalWrite(g2, LOW);
  digitalWrite(y2, LOW);
}

void GRE_ON2(){
  Serial.println("GREEN ON"); // 서랍 열림 상태
  digitalWrite(r2, LOW);
  digitalWrite(g2, HIGH);
  digitalWrite(y2, LOW);
}
void YEL_ON2(){
  Serial.println("YELLOW ON"); //서랍 닫힘 상태
  digitalWrite(r2, LOW);
  digitalWrite(g2, LOW);
  digitalWrite(y2, HIGH);
}
//2번 LED 점등 제어 끝

//LED 효과에 따른 중복 행동 방지 변수
int before1 = 000;
int after1 = 000;
int before2 = 000;
int after2 = 000;
  
void loop(){
  if(Serial.available()){
    char in_data; //라즈베리파이에서 받은 값
    in_data = Serial.read();
      
////////////////////////////////////////////////////////////////////////////////// 1번 서랍 동작

    if(in_data=='1'){ // 1번 닫힘
      before1=after1;
      after1=001;
      if(before1==after1){
        Serial.print("Number 1 can't do");
        Serial.print("\t");
      }
      else{
        OFF1(); //1번 LED 모두 끄고
        YEL_ON1(); //1번 닫힘 노랑
        delay(500);
        digitalWrite(motor1,LOW); 
        analogWrite(speed1,250); //120
        delay(1000); 
        analogWrite(speed1,0);  
      }
      Serial.print("Number 1 is Close\n\n");
   }
   
   else if(in_data=='2'){ //1번 열림
      before1=after1;
      after1=010;
      if(before1==after1){       
        Serial.print("Number 1 can't do");
        Serial.print("\t");
      }
      else{
        OFF1(); //1번 LED 모두 끄고
        GRE_ON1();//1번 열림 초록
        delay(500);        
        digitalWrite(motor1,HIGH); 
        analogWrite(speed1,250);
        delay(1000); 
        analogWrite(speed1,0);  
      }
      Serial.print("Number 1 is Open\n\n");
   }
    
   if(in_data=='5'){ //1번 잠금
      before1=after1;
      after1=100;
      if(before1==after1){
        Serial.print("Number 1 can't do");
        Serial.print("\t");
      }
      else{
        OFF1(); //1번 LED 모두 끄고      
        RED_ON1(); //1번 잠금 빨강
        delay(500);        
        value1 -= 15; //오른쪽으로 30도 값
        lock_device1.write(value1); //잠금 실행
      }
      Serial.print("Number 1 is Lock\n\n");
   }
   
   else if(in_data=='6'){ //1번 잠금 해제
      before1=after1;
      after1=001;
      if(before1==after1){
        Serial.print("Number 1 can't do");
        Serial.print("\t");
      }
      else{
        OFF1(); //1번 LED 모두 끄고      
        YEL_ON1(); //1번 닫힘 노랑 (잠금 해제)
        delay(500);        
        value1 += 15; //왼쪽으로 30도 값
        lock_device1.write(value1); //잠금해제 실행
      }
      Serial.print("Number 1 is UnLock\n\n");
    }
   
////////////////////////////////////////////////////////////////////////////////// 2번 서랍 동작
    
    if(in_data=='3'){ //2번 닫힘
       before2=after2;
       after2=001;
       if(before2==after2){
         Serial.print("Number 2 can't do");
         Serial.print("\t");
       }
       else{
         OFF2(); //2번 LED 모두 끄고        
         YEL_ON2(); //2번 닫힘 노랑
        delay(500);         
         digitalWrite(motor2,LOW); 
         analogWrite(speed2,250);
         delay(1000); 
         analogWrite(speed2,0);  
       }
       Serial.print("Number 2 is Close\n\n");
    }
   
    else if(in_data=='4'){ //2번 열림       
      before2=after2;
      after2=010;
      if(before2==after2){
        Serial.print("Number 2 can't do");
        Serial.print("\t");
      }
      else{
        OFF2(); //2번 LED 모두 끄고        
        GRE_ON2(); //2번 열림 초록 
        delay(500);        
        digitalWrite(motor2,HIGH); 
        analogWrite(speed2,250);
        delay(1000); 
        analogWrite(speed2,0); 
      }
      Serial.print("Number 2 is Open\n\n"); 
    }

    if(in_data=='7'){ //2번 잠금
      before2=after2;
      after2=100;
      if(before2==after2){
        Serial.print("Number 2 can't do");
        Serial.print("\t");
      }
      else{
        OFF2(); //2번 LED 모두 끄고      
        RED_ON2(); //2번 잠금 빨강
        delay(500);        
        value2 -= 15; //오른쪽으로 30도 값
        lock_device2.write(value2); //잠금 실행
      }
      Serial.print("Number 2 is Lock\n\n");
    }

    else if(in_data=='8'){ //2번 잠금 해제    
      before2=after2;
      after2=001;
      if(before2==after2){
        Serial.print("Number 2 can't do");
        Serial.print("\t");
      }
      else{
        OFF2(); //2번 LED 모두 끄고      
        YEL_ON2(); //2번 닫힘 노랑 (잠금 해제)
        delay(500);
        value2 += 15; //왼쪽으로 30도 값
        lock_device2.write(value2); //잠금해제 실행
      }
      Serial.print("Number 2 is UnLock\n\n");
    } 
  }
}
