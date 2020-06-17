#include <SoftwareSerial.h>

SoftwareSerial esp8266(2, 3);
String readStr;

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
  delay(100);
  esp8266.println("AT");
  while(!esp8266.available()) {

  };
  readStr = esp8266.readString();
  Serial.print("Echo:" + readStr);

  if( readStr.indexOf("OK") > -1) {
    Serial.println("AT Command Connection : OK");
  };
}

void loop() {
}
