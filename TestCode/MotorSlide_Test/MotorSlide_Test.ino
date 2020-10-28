#include <AFMotor.h> //L293D 모터드라이브 라이브러리 (따로 추가함)
//디지털 핀 위치가 위쪽 기준 dc모터 위쪽이(A) + 아래가(B) -
//슬라이더의 5V 빨간라인 연결 안했을때 작동은 하나 움직이지 않음 -> 전력이 부족한것으로 예상
//-> 전력부족은 맞음 But 5V 연결해도 부족하여 모터슬라이더 각각에 연결 후 2개씩 같이 동작
//모터슬라이더 테스트 성공
//1번
AF_DCMotor motor1_1(1); //모터쉴드 M1 연결
AF_DCMotor motor1_2(2); //모터쉴드 M2 연결

//2번
AF_DCMotor motor2_1(3); //모터쉴드 M3 연결
AF_DCMotor motor2_2(4); //모터쉴드 M4 연결

void num1_open(){ //1번 서랍 열림
  motor1_1.run(FORWARD);
  motor1_2.run(FORWARD);
  delay(2000);
  motor1_1.run(RELEASE); //BREAK 함수: 급정지
  motor1_2.run(RELEASE); //RELEASE 함수 : 부드럽게 정지(휴식)
}

void num1_close(){ //1번 서랍 닫힘
  motor1_1.run(BACKWARD);
  motor1_2.run(BACKWARD);
  delay(2000);
  motor1_1.run(RELEASE);
  motor1_2.run(RELEASE);
}

void num2_open(){ //2번 서랍 열림
  motor2_1.run(FORWARD);
  motor2_2.run(FORWARD);
  delay(2000);
  motor2_1.run(RELEASE);
  motor2_2.run(RELEASE);
}

void num2_close(){ //2번 서랍 닫힘
  motor2_1.run(BACKWARD);
  motor2_2.run(BACKWARD);
  delay(2000);
  motor2_1.run(RELEASE);
  motor2_2.run(RELEASE);
}


void setup(){
  Serial.begin(9600); //시리얼 통신 설정
  //모터슬라이더 속도 설정
  motor1_1.setSpeed(250); //모터 속도 설정
  motor1_2.setSpeed(250);
  //motor2_1.setSpeed(250);
  //motor2_2.setSpeed(250);
}

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
        num1_close();
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
        num1_open();
      }
      Serial.print("Number 1 is Open\n\n");
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
        num2_close();
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
        num2_open();
      }
      Serial.print("Number 2 is Open\n\n"); 
    }
  }
}
