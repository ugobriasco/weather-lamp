#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "config.h"

//EXP8266
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)
String page = "";

//Lamp handling
int LEDPin = 5;


// Weather API relation
const size_t capacity = 49*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(48) + 11*JSON_OBJECT_SIZE(1) + 49*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(6) + 37*JSON_OBJECT_SIZE(12) + 11*JSON_OBJECT_SIZE(13) + JSON_OBJECT_SIZE(14) + 1600;
DynamicJsonDocument doc(capacity);
int HOURLY_FORECAST = 4;
//const char *host = "http://arduinojson.org/example.json";

// Cronjob
int cron_1 = 60;  //60 Seconds intervall for timer 1;
long t1;

// Cronjobs
void cron1() {
  handleHttpRequest();
}

void cronjob() {
  long tmp = millis();
  if ((t1 + (cron_1 * 1000)) <= tmp) {
    cron1();
    t1 = millis();
  }
}

// Get weather from Openweather API
void handleHttpRequest(void){
  Serial.println("Cronjob 1 active");
  HTTPClient http;
  Serial.println("Getting weather ..");
  http.begin(HOSTNAME);

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  // Serial.print("Returned data from Server:");
  // Serial.println(payload);    //Print request response payload

  if(httpCode == 200)
  {
    //JsonObject root = jsonBuffer.parseObject(payload);

    deserializeJson(doc, payload);
    DeserializationError error = deserializeJson(doc, payload);
    if (error){
      Serial.println("Error by deserializing response body");
    } else {

      JsonArray hourly = doc["hourly"];
      float temp = hourly[HOURLY_FORECAST]["temp"];
      int weatherID = hourly[HOURLY_FORECAST]["weather"][0]["id"]; // 800
      const char* weatherLabel = hourly[HOURLY_FORECAST]["weather"][0]["main"]; // "Clear
      Serial.println("Weather forecast in 4 hours:");
      Serial.print("Temperature: ");
      Serial.println(temp);
      Serial.print("Weather: ");
      Serial.print(weatherID);
      Serial.print("-");
      Serial.println(weatherLabel);

    };
  }
  else
  {
    Serial.println("Error in response");
  }
  http.end();
}

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
  cronjob();
}
