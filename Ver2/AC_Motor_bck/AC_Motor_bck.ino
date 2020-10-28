int motor1 = 12; //모터 1 방향 제어 핀 설정
int speed1 = 10; //모터 1 속도 제어 핀 설정

int motor2 = 13; //모터 2 방향 제어 핀 설정
int speed2 = 11;//모터2 속도 제어 핀 설정

 
void setup(){
  Serial.begin(9600); //시리얼 통신 설정
 
 //모든 핀모드를 OUTPUT으로 설정
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
 
  pinMode(speed1, OUTPUT);
  pinMode(speed2, OUTPUT);
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
        digitalWrite(motor1,LOW); 
        analogWrite(speed1,250); //120
        delay(2000); 
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
        digitalWrite(motor1,HIGH); 
        analogWrite(speed1,250);
        delay(2000); 
        analogWrite(speed1,0);  
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
        digitalWrite(motor2,LOW); 
        analogWrite(speed2,250);
        delay(2000); 
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
      digitalWrite(motor2,HIGH); 
      analogWrite(speed2,250);
      delay(2000); 
      analogWrite(speed2,0); 
     }
     Serial.print("Number 2 is Open\n\n"); 
   }
  }
}
