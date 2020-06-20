#include <SoftwareSerial.h>
#include "config.h"

SoftwareSerial esp8266(3, 2); //RX, TX
String readStr, writeStr;
int incomingByte = 0;   // for incoming serial data
int i = 0;

boolean No_IP=false;
String IP="";
char temp1='0';

String webpage="";
String name="<p>Circuit Digest</p>";   //22
String dat="<p>Data Received Successfully.....</p>";     //21

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

void check4IP(int t1){
  int t2=millis();
  while(t2+t1>millis())
  {
    while(esp8266.available()>0)
    {
      if(esp8266.find("WIFI GOT IP"))
      {
        No_IP=true;
      }
    }
  }
}

void get_ip(){
  IP="";
  char ch=0;
  while(1)
  {
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

void sendSomething(){
  webpage = "<h1>Welcome to Circuit Digest</h1><body bgcolor=f0f0f0>";
  sendwebdata(webpage);
  webpage=name;
  webpage+=dat;
  sendwebdata(webpage);
  delay(1000);
  esp8266.println("AT+CIPCLOSE=0");
}

void setup() {
  Serial.begin(115200);
  Serial.print("Hello");
  esp8266.begin(115200);
  delay(100);
  wifi_init();
}

void loop() {
      while(!esp8266.available()) { };
      readStr = esp8266.readString();
      Serial.print(readStr);

      if(readStr.indexOf("DIS") > -1){
        Serial.print("Arrrrr!!!");
        No_IP = true;
      };

      if(No_IP){
        get_ip();
      }

      if( readStr.indexOf("HTTP") > -1) {
        sendSomething();
        readStr = "";
      };
}
