#define trigPin_FL 5
#define echoPin_FL 6

#define trigPin_F 8
#define echoPin_F 9

#define trigPin_FR 4
#define echoPin_FR 3

int Id=0x100;
long tStart, tEnd, duration, distance;
int Time, FL, F, FR;
byte sendStatus;
bool sendMessage=false;

#include <mcp_can.h>
#include <SPI.h>
MCP_CAN CAN0(10);     // Set CS to pin 10
byte data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


void setup() {
  Serial.begin (500000);
  pinMode(trigPin_FL, OUTPUT);
  pinMode(echoPin_FL, INPUT);  
  pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);  
  pinMode(trigPin_FR, OUTPUT);
  pinMode(echoPin_FR, INPUT); 

//***CAN-Buss setup***

  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) 
    Serial.println("MCP2515 Initialized Successfully!");
  else 
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}

void loop() {
  FL=Read_Distance(trigPin_FL,echoPin_FL);
  F=Read_Distance(trigPin_F,echoPin_F);
  FR=Read_Distance(trigPin_FR,echoPin_FR);

  if(FL>0 && FL <=200){
    data[0]=FL;
    sendMessage=true;
    }
  if(F>0 && F <=200){
    data[1]=F;
    sendMessage=true;
    }
  if(FR>0 && FR <=200){
    sendMessage=true;
    data[2]=FR;
    }
      
  if(sendMessage){
    sendMessage=false;
    SendCanBussMessage();
    }     
 delay(200);
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
  //Test
  }
  
void SendCanBussMessage(){
  byte sndStat = CAN0.sendMsgBuf(Id, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
    Serial.print(data[0]);
    Serial.print(":");
    Serial.print(data[1]);
    Serial.print(":");
    Serial.print(data[2]);
    Serial.print(":");
    Serial.print(data[3]);
    Serial.print(":");
    Serial.print(data[4]);
    Serial.print(":");
    Serial.print(data[5]);
    Serial.print(":");
    Serial.print(data[6]);
    Serial.print(":");
    Serial.println(data[7]);
    clearData();
  } else {
    Serial.println("Error Sending Message...");
  }
 }
 void clearData(){
  for(int i=0;i<8;i++)
    data[i]=0;
  }
