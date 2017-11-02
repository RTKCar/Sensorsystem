#define trigPin_FL 5
#define echoPin_FL 6

#define trigPin_F 8
#define echoPin_F 9

#define trigPin_FR 4
#define echoPin_FR 3

int FL, F, FR, Id;
bool sendMessage= false;
long tStart, tEnd, Time, duration, distance;

#include <mcp_can.h>
#include <SPI.h>
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                            // Array to store serial string
#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
int sendId=0x123;

void setup() {
  Serial.begin (500000);
  pinMode(trigPin_FL, OUTPUT);
  pinMode(echoPin_FL, INPUT);  
  pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);  
  pinMode(trigPin_FR, OUTPUT);
  pinMode(echoPin_FR, INPUT); 

  SetupCANBuss();

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
    SendData();
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
  
 }
  

 void clearData(){
  for(int i=0;i<8;i++)
    data[i]=0;
  }


void SetupCANBuss(){
  Serial.begin(500000);
    
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK){
    Serial.println("MCP2515 Initialized Successfully!");
    }
  else{
    Serial.println("Error Initializing MCP2515...");
   }
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  attachInterrupt(digitalPinToInterrupt(CAN0_INT), ReadMessage, FALLING);// interrupt activates when canbuss message can be read
  
  
}

void ReadMessage(){

    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();
  
}

void SendData(){
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(sendId, 0, 8, data);
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
