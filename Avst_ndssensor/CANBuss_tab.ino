//***CAN-Buss variabes**
#include <mcp_can.h>
#include <SPI.h>
bool messageRecieved = false;
int recievedMessage[8];
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                            // Array to store serial string
#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10
byte data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
int sendId;

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
    else{
     // sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
    }
  
    //Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
     // Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        //sprintf(msgString, " 0x%.2X", rxBuf[i]);
        sprintf(msgString, " %d", rxBuf[i]);
        recievedMessage[i]=atoi(msgString);     

        
        //recievedMessage[i]=atoi(msgString);
        //Serial.println(recievedMessage[i]);
        //Serial.print(msgString);
      }
    }
    messageRecieved=true;    
    Serial.println();
  
}

bool messageAvalible(){

  return messageRecieved;

}

void getMessage(int message[]){
  messageRecieved=false;
  for(int i=0;i<8;i++)
  message[i]=recievedMessage[i];
  }  

int getRecievedId(){
return rxId;  
}
void SendData(){
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(sendId, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
}
void setData(char Data[]){
  for(int i=0;i<8;i++)
    data[i]=Data[i];
  
  }
void printdata(){
  for(int i=0;i<8;i++){
    Serial.print(data[i]);
    Serial.print(":");
  }
  
  }  

void setSendId(int id){
  sendId=id;
  }   
 
