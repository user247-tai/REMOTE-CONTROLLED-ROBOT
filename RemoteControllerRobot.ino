#define ena   13     // ENA ==> GPIO 13     
#define enb   14     // ENB ==> GPIO 14    
#define in1  15      // IN1 ==> GPIO 15
#define in2  16      // IN2 ==> GPIO 16   
#define in3  17      // IN3 ==> GPIO 17     
#define in4  18      // IN4 ==> GPIO 18     
#define led 4        // LED ==> GPIO 4
#define servoPin 19 // Servo ==> GPIO 19

#include <ESP32Servo.h>
#include <WiFi.h> 
#include <WebServer.h>
WiFiClient client;
WebServer server(80);

/* WIFI settings */
const char* ssid = "ESP WiFi";
const char* password = "12345678";
// Temp Variable
String  data =""; /* data received from application */
int ledState = 0;
Servo MyServo;

void setup()
{
  /* initialize motor control pins as output */
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT); 
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(led, OUTPUT);
  MyServo.attach(servoPin);
  MyServo.write(90);
  Serial.begin(9600);
  //connectWiFi();
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 // Starting WEB-server 
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();  
}
void loop()
{
    server.handleClient(); 
    data = server.arg("State");
    Control(data);
} 
void STOP(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}
void BACKWARD(){
  analogWrite(ena, 100);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); 
  analogWrite(enb, 100); 
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void TURN_LEFT(){
  analogWrite(ena, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enb, 255); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void TURN_RIGHT(){
  analogWrite(ena, 255); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enb, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void FORWARD(){
  analogWrite(ena, 100);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enb, 100); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);  
}
void HTTP_handleRoot(void) {
  if( server.hasArg("State") ){
    Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
void Control(String data){
  //Control Servo
  if (data == "0"){
      Serial.println("S0");
      MyServo.write(0);
  }
  else if (data == "1"){
    Serial.println("S1");
    MyServo.write(90);  
  }
  else if (data == "2"){
    Serial.println("S2");
    MyServo.write(180);  
  }
  //Control LED
  if (data == "onled"){
     Serial.println("on");
     ledState = 1;
  }
  else if (data == "offled"){
    Serial.println("off");
    ledState = 0;
  } 
  if (ledState == 1) digitalWrite(led, HIGH);
  else if (ledState == 0) digitalWrite(led, LOW);
  // Control Motors
  if (data == "forward"){
    Serial.print("F");
    Serial.println(ledState);
    FORWARD();
  }
  else if (data == "backward"){
    Serial.print("B");
    Serial.println(ledState);
    BACKWARD();
  }
  else if (data == "left"){
    Serial.print("L");
    Serial.println(ledState);
    TURN_LEFT();
  }
  else if (data == "right"){
    Serial.print("R");
    Serial.println(ledState);
    TURN_RIGHT();
  }
  else if(data == "stop"){
    Serial.print("S");
    Serial.println(ledState);
    STOP();
  };  
}
