#include <AFMotor.h> //L293D 모터드라이브 라이브러리 (따로 추가함)
//디지털 핀 위치가 위쪽 기준 dc모터 위쪽이(A) + 아래가(B) -
//슬라이더의 5V 빨간라인 연결 안했을때 작동은 하나 움직이지 않음 -> 전력이 부족한것으로 예상
AF_DCMotor motor1(1);
AF_DCMotor motor2(4);

void num1_close(){ //1번 서랍 닫힘
  motor1.run(FORWARD);
  delay(1000);
  motor1.run(RELEASE);
}

void num1_open(){ //1번 서랍 열림
  motor1.run(BACKWARD);
  delay(1000);
  motor1.run(RELEASE);
}

void num2_close(){ //2번 서랍 닫힘
  motor2.run(FORWARD);
  delay(1000);
  motor2.run(RELEASE);
}

void num2_open(){ //2번 서랍 열림
  motor2.run(BACKWARD);
  delay(1000);
  motor2.run(RELEASE);
}

void setup(){
  Serial.begin(9600); //시리얼 통신 설정
  //모터슬라이더 속도 설정
  motor1.setSpeed(250);
  motor2.setSpeed(250);
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
        num1_open();
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
        num1_close();
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
