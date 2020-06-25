#include <SoftwareSerial.h>
#include "config.h"

#define RX_PIN 3
#define TX_PIN 2
#define DUTY_LED_PIN 8
#define LAMP_R_PIN 13
#define LAMP_G_PIN 11
#define LAMP_B_PIN 12


SoftwareSerial esp8266(RX_PIN, TX_PIN); //RX, TX
String readStr; // IO-Carrier


/*
* 1) ESP8266 handler
*/

void connect_wifi(String cmd, int t){
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

void get_ip(){

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

void wifi_init(){

  connect_wifi("AT",100);
  connect_wifi("AT+CWMODE=3",100);
  connect_wifi("AT+CWQAP",100);
  connect_wifi("AT+RST",5000);
  Serial.println("Connecting Wifi....");
  connect_wifi(AP,7000);
  delay(5000);
  get_ip();
  connect_wifi("AT+CIPMUX=1",100);
  connect_wifi("AT+CIPSERVER=1,80",100);
}

void sendwebdata(String webPage){
    int ii=0;
     while(1)
     {
      unsigned int l=webPage.length();
      Serial.print("AT+CIPSEND=0,");
      esp8266.print("AT+CIPSEND=0,");
      Serial.println(l+2);
      esp8266.println(l+2);
      delay(100);
      Serial.println(webPage);
      esp8266.println(webPage);
      while(esp8266.available())
      {
        //Serial.print(Serial.read());
        if(esp8266.find("OK"))
        {
          ii=11;
          break;
        }
      }
      if(ii==11)
      break;
      delay(100);
     }
}

/*
* 2) HTTP responses handler
*/

void sendSomething(){
  sendwebdata("<h1>Welcome to The weather Lamp</h1>");
  delay(1000);
  esp8266.println("AT+CIPCLOSE=0");
}

void sendStatus(){
  sendwebdata("{'key': 'foo'}");
  delay(1000);
  esp8266.println("AT+CIPCLOSE=0");
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
void RGB_color(int red_light_value, int green_light_value, int blue_light_value){
  analogWrite(LAMP_R_PIN, red_light_value);
  analogWrite(LAMP_G_PIN, green_light_value);
  analogWrite(LAMP_B_PIN, blue_light_value);
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

  RGB_color(255,0,0);
  wifi_init();
}

void loop() {
      while(!esp8266.available()) { };

      readStr = esp8266.readString();
      Serial.print(readStr);

      if( readStr.indexOf("HTTP") > -1) {
        if(readStr.indexOf("status") > -1) {
          sendStatus();
        } else {
          sendSomething();
        }
      };
      readStr = "";
}
