#define trigPin_FL 5  //yeelow
#define echoPin_FL 6  //green

#define trigPin_F 8   //blue
#define echoPin_F 9   //grey

#define trigPin_FR 4  //white
#define echoPin_FR 3  //green

#include <mcp_can.h>
#include <SPI.h>

int FL, F, FR, Id;
bool sendMessage= false;
long tStart, tEnd, Time, duration, distance;
char Data[8]={0, 0, 0, 0, 0, 0, 0, 0};
void setup() {
  SetupCANBuss();
  Serial.begin (500000);
  pinMode(trigPin_FL, OUTPUT);
  pinMode(echoPin_FL, INPUT);  
  pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);  
  pinMode(trigPin_FR, OUTPUT);
  pinMode(echoPin_FR, INPUT); 

}

void loop() {
  FL=Read_Distance(trigPin_FL,echoPin_FL);
  F=Read_Distance(trigPin_F,echoPin_F);
  FR=Read_Distance(trigPin_FR,echoPin_FR);

  if(FL>0 && FL <=200){
    Data[0]=FL;
    sendMessage=true;
    }
  if(F>0 && F <=200){
    Data[1]=F;
    sendMessage=true;
    }
  if(FR>0 && FR <=200){
    sendMessage=true;
    Data[2]=FR;
    }
      
  if(sendMessage){
    setData(Data);
    sendMessage=false;
    SendData();
    Serial.print("Sent message: ");
    printdata();
    Serial.println();
    ClearData();
    printdata();
    }     
 delay(500);
 Serial.println("********************************");
}
int Read_Distance(int trigPin, int echoPin){
  
  tStart=millis();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH,20000);
  distance= duration*0.034/2;
  tEnd=millis();
  Time=tEnd-tStart;
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Time: ");
  Serial.println(Time);
  
  return distance;
  
 }
  

 
void ClearData(){

    for(int i=0;i<8;i++)
      Data[i]=0x00;
} 



