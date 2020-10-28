#include <Servo.h> //서보모터 라이브러리
#include <AFMotor.h> //L293D 모터드라이브 라이브러리 (따로 추가함)

//ALL TEST 성공!

//1번
AF_DCMotor motor1_1(1); //모터쉴드 M1 연결
AF_DCMotor motor1_2(3); //모터쉴드 M3 연결
//2번
AF_DCMotor motor2_1(2); //모터쉴드 M2 연결
AF_DCMotor motor2_2(4); //모터쉴드 M4 연결

Servo lock_device1; //1번 잠금 서보
int value1 = 90; //1번 초기 각도
Servo lock_device2; //2번 잠금 서보
int value2 = 90; //2번 초기 각도

void num1_open(){ //1번 서랍 열림
  motor1_1.run(FORWARD);
  motor1_2.run(FORWARD);
  delay(1000);
  motor1_1.run(RELEASE); //BREAK 함수: 급정지
  motor1_2.run(RELEASE); //RELEASE 함수 : 부드럽게 정지(휴식)
}

void num1_close(){ //1번 서랍 닫힘
  motor1_1.run(BACKWARD);
  motor1_2.run(BACKWARD);
  delay(1000);
  motor1_1.run(RELEASE);
  motor1_2.run(RELEASE);
}

void num2_open(){ //2번 서랍 열림
  motor2_1.run(FORWARD);
  motor2_2.run(FORWARD);
  delay(1000);
  motor2_1.run(RELEASE);
  motor2_2.run(RELEASE);
}

void num2_close(){ //2번 서랍 닫힘
  motor2_1.run(BACKWARD);
  motor2_2.run(BACKWARD);
  delay(1000);
  motor2_1.run(RELEASE);
  motor2_2.run(RELEASE);
}

void setup(){
  Serial.begin(9600); //시리얼 통신 설정

  //모터슬라이더 속도 설정
  motor1_1.setSpeed(250); //모터 속도 설정
  motor1_2.setSpeed(250);
  motor2_1.setSpeed(250);
  motor2_2.setSpeed(250);

  //서보모터 핀모드 OUTPUT으로 설정
  lock_device1.attach(10);  // 1번 서보모터
  lock_device2.attach(9);  // 2번 서보모터

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

//1번 잠금
void num1_lock(){
  value1 -= 30; //오른쪽으로 30도
  lock_device1.write(value1); //잠금 실행
}
//1번 잠금 해제
void num1_unlock(){
  value1 += 30; //왼쪽으로 30도 값
  lock_device1.write(value1); //잠금해제 실행
}
//2번 잠금
void num2_lock(){
  value2 -= 30; //오른쪽으로 30도 값
  lock_device2.write(value2); //잠금 실행
}
//2번 잠금해제
void num2_unlock(){
  value2 += 30; //왼쪽으로 30도 값
  lock_device2.write(value2); //잠금해제 실행
}

//1번 LED 점등 제어
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
        YEL_ON1(); //1번 닫힘 노랑
        num1_close(); //1번 닫힘
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
        GRE_ON1();//1번 열림 초록
        num1_open(); //1번 열림
      }
      Serial.print("Number 1 is Open\n\n");
   }
    
   if(in_data=='5'){ //1번 잠금
      before1=after1;
      after1=100;
      if(before1==after1){ //예외처리 추가해야함.
        Serial.print("Number 1 can't do");
        Serial.print("\t");
      }
      else{
        RED_ON1(); //1번 잠금 빨강
        num1_lock(); //1번 잠금
      }
      Serial.print("Number 1 is Lock\n\n");
   }
   
   else if(in_data=='6'){ //1번 잠금 해제
      before1=after1;
      after1=001;
      if(before1==after1){//예외처리 추가해야함.
        Serial.print("Number 1 can't do");
        Serial.print("\t");
      }
      else{
        YEL_ON1(); //1번 닫힘 노랑 (잠금 해제)
        num1_unlock(); //1번 잠금 해제
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
        YEL_ON2(); //2번 닫힘 노랑
        num2_close(); //2번 닫힘
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
        GRE_ON2(); //2번 열림 초록 
        num2_open(); //2번 열림
      }
      Serial.print("Number 2 is Open\n\n"); 
    }

    if(in_data=='7'){ //2번 잠금
      before2=after2;
      after2=100;
      if(before2==after2){//예외처리 추가해야함.
        Serial.print("Number 2 can't do");
        Serial.print("\t");
      }
      else{
        RED_ON2(); //2번 잠금 빨강
        num2_lock(); //2번 잠금
      }
      Serial.print("Number 2 is Lock\n\n");
    }

    else if(in_data=='8'){ //2번 잠금 해제    
      before2=after2;
      after2=001;
      if(before2==after2){//예외처리 추가해야함.
        Serial.print("Number 2 can't do");
        Serial.print("\t");
      }
      else{
        YEL_ON2(); //2번 닫힘 노랑 (잠금 해제)
        num2_unlock(); //2번 잠금 해제
      }
      Serial.print("Number 2 is UnLock\n\n");
    } 
  }
}
