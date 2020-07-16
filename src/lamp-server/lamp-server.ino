#include <SoftwareSerial.h>
#include "config.h"

#define RX_PIN 3
#define TX_PIN 2
#define DUTY_LED_PIN 8
#define LAMP_R_PIN 13
#define LAMP_G_PIN 11
#define LAMP_B_PIN 12
//uncomment this line if using a Common Anode LED
//#define COMMON_ANODE


SoftwareSerial esp8266(RX_PIN, TX_PIN); //RX, TX
String readStr; // IO-Carrier
String lampState = "";

// Lamp Global variables
int r = 0;
int g = 0;
int b = 0;
String effect = "";
/*
* 1) ESP8266 handler
*/

void sendCommand(String cmd, int t){
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);
    esp8266.println(cmd);
    while(esp8266.available())
    {
      if(esp8266.find("OK"))
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  Serial.println("OK");
  else
  Serial.println("Error");
}

void getIP(){
  String  IP="";
  char ch=0;
  while(1)
  {
    blinkDutyLed(100);
    delay(3000);
    Serial.println("IP, where are you?");
    esp8266.println("AT+CIFSR");
    while(esp8266.available()>0){
      Serial.println(esp8266.read());
      if(esp8266.find("STAIP,")){
        delay(1000);
        while(esp8266.available()>0){
          ch=esp8266.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+'){
        break;
      }

    }
    if(ch=='+') {
      break;
    }
    delay(1000);
  }
  Serial.print("IP Address: ");
  Serial.println(IP);
  Serial.print("Port: ");
  Serial.println(80);
}

void initESP8266(){
  sendCommand("AT",100);
  sendCommand("AT+CWMODE=3",100);
  sendCommand("AT+CWQAP",100);
  sendCommand("AT+RST",5000);
  //connectToAP();
  sendCommand("AT+CIPMUX=1",100);
  sendCommand("AT+CIPSERVER=1,80",100);
}

void connectToAP(){
  Serial.println("Connecting Wifi....");
  sendCommand(AP,7000);
  delay(7000);
  //getIP();
  esp8266.println("AT+CIFSR");
  while(esp8266.available()>0){
    Serial.println(esp8266.readString());
  }
}

void sendToClient(int SessionNo, String s){
    Serial.println("\nAT+CIPSEND=" + String(SessionNo) + "," + String(s.length()+2));
    delay(100);
    esp8266.println("AT+CIPSEND=" + String(SessionNo) + "," +  String(s.length()+2) );
    while(!esp8266.available()) { };
    readStr = esp8266.readString();
    Serial.print(readStr);
    delay(100);
    Serial.println("\n" + s +"\n\r");
    esp8266.println(s);
    return;
}

void closeSession(int SessionNo){
   esp8266.println("AT+CIPCLOSE=" + String(SessionNo));
   while(!esp8266.available()) {  };
   readStr = esp8266.readString();
   Serial.print(readStr);
   return;
}

/*
* 2) HTTP responses handler
*/

void sendStatus(){
  // Send the page
  sendToClient(0,"{\"message\":\"All good\"}");
  delay(500);
  closeSession(0);
  delay(500);
}

/*
* 3) Light handler
*/

void blinkDutyLed(int period_ms){
    digitalWrite(DUTY_LED_PIN, HIGH);
    delay(period_ms / 2);
    digitalWrite(DUTY_LED_PIN, LOW);
    delay(period_ms / 2);
}

// Handle weatherlamp
void setRGBlamp(int r, int g, int b){
  #ifdef COMMON_ANODE
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  #endif
  analogWrite(LAMP_R_PIN, r);
  analogWrite(LAMP_G_PIN, g);
  analogWrite(LAMP_B_PIN, b);
}

void setRGB(int _r, int _g, int _b){
  r = _r;
  g = _g;
  b = _b;
}


void setLampState(String str) {

  // Temperature
  if(str.indexOf("very-cold") > -1){
    setRGB(0, 0, 255);  //deep blue
  }

  if(str.indexOf("cold") > -1){
    setRGB(0, 255, 255); //light blue
  }

  if(str.indexOf("mild") > -1){
    setRGB(0, 255, 0); // green
  }

  if(str.indexOf("hot") > -1){
    setRGB(200, 255, 0); // yellow
  }

  if(str.indexOf("very-hot") > -1){
    setRGB(255, 0, 0); // red
  }




  // Weather
  if(str.indexOf("rain") > -1){
      effect = "rain";
  }

  if(str.indexOf("clear") > -1){
      effect = "clear";
  }

}

void playEffect(){
  if(effect.indexOf("rain") > -1){
    delay(200);
    setRGBlamp(r,g,b);
    delay(200);
    if(g > 100){
      setRGBlamp(r,0,b);
    } else {
      setRGBlamp(r,255,b);
    }
  }

  else {
    setRGBlamp(r,g,b);
  }
}

/*
* Main
*/

void setup() {
  // Led declaration
  pinMode(8, OUTPUT);
  pinMode(LAMP_R_PIN, OUTPUT);
  pinMode(LAMP_G_PIN, OUTPUT);
  pinMode(LAMP_B_PIN, OUTPUT);

  // Initialization
  blinkDutyLed(1000);
  Serial.begin(115200);
  Serial.print("Hello");
  esp8266.begin(115200);
  delay(100);

  //initESP8266();
}

void loop() {

  playEffect();

  // Serial COM handler
  while(Serial.available() > 0){
    readStr = Serial.readString();
    setLampState(readStr);
    Serial.println(effect);
    readStr = "";
  }

  // ESP8266 handler
  while(esp8266.available() > 0) {
    readStr = esp8266.readString();
    Serial.println(readStr);
    if(readStr.indexOf("HTTP") > -1) {
      if(readStr.indexOf("status") > -1) {
        sendStatus();
      } else {
        setLampState(readStr);
        sendStatus();
      }
    };
    readStr = "";
  };
}
