// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
// Replace with your network credentials
const char* ssid = "linux";
const char* password = "linux123";

const int room1 = 2;
const int room2 = 0;
String value = "0";
const char* PARAM_INPUT = "value";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 Serial.println(WiFi.localIP());

  // Send a GET request to <ESP_IP>/slider1?value1=<inputMessage>
  server.on("/room1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/room1?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      value = inputMessage;
      analogWrite(room1, value.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Send a GET request to <ESP_IP>/room2?value=<inputMessage>
  server.on("/room2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input2 value on <ESP_IP>/room2?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      value = inputMessage;
      analogWrite(room2, value.toInt());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  
  // Start server
  server.begin();
} 
void loop() {
}
