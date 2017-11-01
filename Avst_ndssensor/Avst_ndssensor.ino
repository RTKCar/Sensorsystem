#define trigPin_FL 5
#define echoPin_FL 6

#define trigPin_F 8
#define echoPin_F 9

#define trigPin_FR 4
#define echoPin_FR 3

long tStart, tEnd, duration, distance;
int Time;
void setup() {
  Serial.begin (500000);
  pinMode(trigPin_FL, OUTPUT);
  pinMode(echoPin_FL, INPUT);  
  pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);  
  pinMode(trigPin_FR, OUTPUT);
  pinMode(echoPin_FR, INPUT); 
}

void loop() {
  //Read_Distance(trigPin_FL,echoPin_FL);
  //Read_Distance(trigPin_F,echoPin_F);
  Read_Distance(trigPin_FR,echoPin_FR);
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

