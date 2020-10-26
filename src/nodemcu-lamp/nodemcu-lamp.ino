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
int rPin = 12;
int bPin = 13;
int gPin = 14;



// Weather API relation
const size_t capacity = 49*JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(48) + 11*JSON_OBJECT_SIZE(1) + 49*JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(6) + 37*JSON_OBJECT_SIZE(12) + 11*JSON_OBJECT_SIZE(13) + JSON_OBJECT_SIZE(14) + 1600;
DynamicJsonDocument doc(capacity);
float temp = 0;
int weatherID = 0;


// Cronjobs
int cron_1 = 300;  //300 Seconds (5 min) intervall for timer 1;
int cron_2 = 600;  //600 Seconds (10 min) intervall for timer 1;
    long t1;
    long t2;
void cron1() {
  getWeatherData();
  setWeatherLamp();
}
void cron2() {
  printUsedWiFi();
}
void cronjobs() {
  long tmp = millis();
  if ((t1 + (cron_1 * 1000)) <= tmp) {
    cron1();
    t1 = millis();
  }
  if ((t2 + (cron_2 * 1000)) <= tmp) {
    cron2();
    t2 = millis();
  }
}

// Logging
void printUsedWiFi(void){
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Get weather from Openweather API and parse data in `temp` and `weatherID`.
void getWeatherData(void){
  Serial.println("Cronjob 1 active");
  HTTPClient http;
  Serial.println("Getting weather ..");
  http.begin(HOSTNAME);

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload from server

  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code

  if(httpCode == 200)
  {
    deserializeJson(doc, payload);
    DeserializationError error = deserializeJson(doc, payload);
    if (error){
      Serial.println("Error by deserializing response body");
    } else {

      JsonArray hourly = doc["hourly"];
      temp = hourly[FORECAST_IN_HOURS]["feels_like"]; //14.4
      weatherID = hourly[FORECAST_IN_HOURS]["weather"][0]["id"]; // 800
      const char* weatherLabel = hourly[FORECAST_IN_HOURS]["weather"][0]["description"]; // "Clear
      Serial.print("Weather forecast in ");
      Serial.print(FORECAST_IN_HOURS);
      Serial.println(" hours:");
      Serial.print("Temperature (feels-like):\t");
      Serial.print(temp);
      Serial.println("C");
      Serial.print("Weather:\t\t");
      Serial.print(weatherID);
      Serial.print("-");
      Serial.println(weatherLabel);

      //Update page
      page = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'/></head><body><h1>THE WEATHERLAMP!</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p><div><p>";
      page.concat("Weather forecast in ");
      page.concat(FORECAST_IN_HOURS);
      page.concat("hours:</p><p>Temperature (Feels-like):");
      page.concat( temp);
      page.concat(" C</p><p>Weather:");
      page.concat(weatherID);
      page.concat("-");
      page.concat(weatherLabel);
      page.concat("</p></div></body><html>");
    };
  }
  else
  {
    Serial.println("Error in response");
  }
  http.end();
}

void setWeatherLamp(void){
  // Mock weather
  if(weatherID < 700 && weatherID != 0){
    digitalWrite(LEDPin, HIGH);
    loopColors(10);
  } else {
    digitalWrite(LEDPin, LOW);
  }
  return;
}

// Run webserver to handle the lamp manually.
void startWebServer(void){

  page = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no'/></head><body><h1>THE WEATHERLAMP!</h1><p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p><body><html>";
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


void loopColors(int fadeSpeed){
  int r, g, b;

  // fade from blue to violet
  for (r = 0; r < 256; r++) {
    analogWrite(rPin, r);
    delay(fadeSpeed);
  }
  // fade from violet to red
  for (b = 255; b > 0; b--) {
    analogWrite(bPin, b);
    delay(fadeSpeed);
  }
  // fade from red to yellow
  for (g = 0; g < 256; g++) {
    analogWrite(gPin, g);
    delay(fadeSpeed);
  }
  // fade from yellow to green
  for (r = 255; r > 0; r--) {
    analogWrite(rPin, r);
    delay(fadeSpeed);
  }
  // fade from green to teal
  for (b = 0; b < 256; b++) {
    analogWrite(bPin, b);
    delay(fadeSpeed);
  }
  // fade from teal to blue
  for (g = 255; g > 0; g--) {
    analogWrite(gPin, g);
    delay(fadeSpeed);
  }
  // fade from blue to dark
  for (b = 255; b > 0; b--) {
    analogWrite(bPin, b);
    delay(fadeSpeed);
  }

}

//main
void setup(void){

  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  //RGB
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);

  loopColors(5);

  delay(1000);
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  printUsedWiFi();
  startWebServer();
  getWeatherData();
}

void loop(void){
  server.handleClient();
  cronjobs();
}
