#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "config.h"

#define SSID "Pretty Fly for a Wi-Fi"
#define PASSWORD "Bonaventur4"

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
int LEDPin = 5;
String page = "";

StaticJsonDocument<1024> doc;




void setup(void){

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  delay(1000);
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // Run webserver
  page = "<h1>THE WEATHERLAMP!</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";

  server.on("/", [](){
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", [](){
    server.send(200, "text/html", page);
    digitalWrite(LEDPin, HIGH);
    delay(1000);
  });
  server.on("/LEDOff", [](){
    server.send(200, "text/html", page);
    digitalWrite(LEDPin, LOW);
    delay(1000);
  });
  server.begin();
  Serial.println("Web server started!");
}

void loop(void){
  server.handleClient();

  HTTPClient http;
  Serial.println("Getting weather ..");
  http.begin(HOSTNAME);

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload

  if(httpCode == 200)
  {
    //JsonObject root = jsonBuffer.parseObject(payload);

    deserializeJson(doc, payload);
    DeserializationError error = deserializeJson(doc, payload);
    if (error){
      Serial.println("Error by deserializing response body");
      return;
    }

    Serial.println(doc);

    const char* current = doc["current"];
    Serial.println(current);






   //  // Allocate JsonBuffer
   //  // Use arduinojson.org/assistant to compute the capacity.
   //  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
   //  DynamicJsonBuffer jsonBuffer(capacity);
   //
   // // Parse JSON object
   //  JsonObject& root = jsonBuffer.parseObject(payload);
   //  if (!root.success()) {
   //    Serial.println(F("Parsing failed!"));
   //    return;
   //  }
   //
   //  // Decode JSON/Extract values
   //  Serial.println(F("Response:"));
   //  Serial.println(root["current"].as<char*>());
  }
  else
  {
    Serial.println("Error in response");
  }


  http.end();

  delay(60000);




}
