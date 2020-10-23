int motor1 = 12; //모터 1 방향 제어 핀 설정
int speed1 = 10; //모터 1 속도 제어 핀 설정

int motor2 = 13; //모터 2 방향 제어 핀 설정
int speed2 = 11;//모터2 속도 제어 핀 설정

int potValue=A0; 
int potValue2=A1; 
 
int directStatus = false;
int directStatus2 = false;
 
void setup(){
  Serial.begin(9600); //시리얼 통신 설정
 
 //모든 핀모드를 OUTPUT으로 설정
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
 
  pinMode(speed1, OUTPUT);
  pinMode(speed2, OUTPUT);
}
 
void loop(){
  
    if(Serial.available()){
      char in_data;
      in_data = Serial.read();
      int potVal = analogRead(A0); // 가변 저항 값을 potVal에 담음.
      int potVal2 = analogRead(A1);

 //슬라이더가 끝에 도달했을음 인식 하는 부분 
      if(potVal == 0 || potVal == 1023){ //가변 저항의 값이 0또는 1023이라면 directStatus 상태 변경
        if(directStatus == false){
          directStatus = true;
        }//|| potVal == 1 || potVal == 2 || potVal == 3 || 
        //potVal == 1020 || potVal == 1021 || potVal == 1022 오차 범위
        else directStatus=false;
      }
      if(potVal2 == 0 || potVal2 == 1023){ //가변 저항의 값이 0또는 1023이라면 directStatus 상태 변경
        if(directStatus2 == false){
          directStatus2 = true;
        }
        else directStatus2=false;
      }
   
   if(in_data=='1'){ //모터가 있는 방향으로 슬라이더 움직임
        if(potVal == 1023){
          Serial.print("Number 1 can't do");
          Serial.print("\t");
        }
        digitalWrite(motor1,LOW); 
        analogWrite(speed1,250); //120
        delay(1000); 
        analogWrite(speed1,0);
        Serial.print("Number 1 is Close\n\n");
   }
    else if(in_data=='2'){ //모터가 있는 반대 방향으로 슬라이더 움직임
        if(potVal == 0){
          Serial.print("Number 1 can't do");
          Serial.print("\t");
        }
        digitalWrite(motor1,HIGH); 
        analogWrite(speed1,250);
        delay(1000); 
        analogWrite(speed1,0);
        Serial.print("Number 1 is Open\n\n");
    }    
      
    if(in_data=='3'){ //모터가 있는 방향으로 슬라이더 움직임
        if(potVal2 == 1023){
          Serial.print("Number 2 can't do");
          Serial.print("\t");
        }
        digitalWrite(motor2,LOW); 
        analogWrite(speed2,250);
        delay(1000); 
        analogWrite(speed2,0);
        Serial.print("Number 2 is Close\n\n");
        
   }
    else if(in_data=='4'){ //모터가 있는 반대 방향으로 슬라이더 움직임
        if(potVal2 == 0){
          Serial.print("Number 2 can't do");
          Serial.print("\t");
        }
        digitalWrite(motor2,HIGH); 
        analogWrite(speed2,250);
        delay(1000); 
        analogWrite(speed2,0);
        Serial.print("Number 2 is Open\n\n"); 
        
    }/*
    
      Serial.print("Number1 potVal : "); 
      Serial.print(potVal); 
      Serial.print('\t');
      Serial.print("direction : "); 
      Serial.println(directStatus);
    */
  
 
    
  /* serial print
 //potVal의 값과 방향 상태를 시리얼 모니터 창에 출력한다. 
    Serial.print("Number1 potVal : "); 
    Serial.print(potVal); 
    Serial.print('\t');
    Serial.print("direction : "); 
    Serial.println(directStatus);

    Serial.print("Number2 potVal : "); 
    Serial.print(potVal2); 
    Serial.print('\t');
    Serial.print("direction : "); 
    Serial.println(directStatus2);

    */  
    }
    
  
    
  
  
}
