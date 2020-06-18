#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial esp8266(2, 3);
String message;
bool messageReady = false;
int inByte = 0;

void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
  delay(100);
  esp8266.println("AT+GMR");

  pinMode(8, OUTPUT);
}

void loop() {

  // Serial communication handler
  while(Serial.available() > 0){
    message = Serial.readString();
    messageReady = true;
  }

  if(messageReady){
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, message);

    if(error){
      Serial.print("Deserialization Failed");
      Serial.print(error.c_str());
      messageReady = false;
      return;
    }

    if(doc["type"] == "request"){
      doc["type"] = "response";
      doc["message"] = "Hello!";
      sayHello();
      serializeJson(doc, Serial);
    }
    messageReady = false;
  }
  if(esp8266.available()) {
    Serial.println("[ESP8266] " + esp8266.readString());
  }
}

void sayHello() {
  digitalWrite(8, HIGH);
  delay(100);
  digitalWrite(8, LOW);
  return;
};

void init_esp8266(){
  //Check AT Connection

  // Set Device Mode

  // Connect AP

  return;

}
