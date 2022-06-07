#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "linux";
const char* password = "linuxxxx";

const char* PARAM_INPUT_2 = "state";
const int output_1 = 0;
const int output_2 = 2;
const int buttonPin_1 = 5;
const int buttonPin_2 = 4;

// Variables will change:
int ledState_1 = LOW;          // the current state of the output_1 pin
int ledState_2 = LOW;          // the current state of the output_2 pin
int buttonState_1;             // the current reading from the input pin
int buttonState_2;             // the current reading from the input pin
int lastbuttonState_1 = LOW;   // the previous reading from the input pin
int lastbuttonState_2 = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime_1 = 0;  // the last time the output_1 pin was toggled
unsigned long lastDebounceTime_2 = 0;  // the last time the output_2 pin was toggled
unsigned long debounceDelay_1 = 50;   // the debounce time; increase if the output_1 flickers
unsigned long debounceDelay_2 = 50;   // the debounce time; increase if the output_2 flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html_2[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<title>ESP Web Server</title>
</html>
)rawliteral";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(output_1, OUTPUT);
  pinMode(output_2, OUTPUT);
  digitalWrite(output_1, LOW);
  digitalWrite(output_2, LOW);
  pinMode(buttonPin_1, INPUT);
  pinMode(buttonPin_2, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Send a GET request to <ESP_IP>/room_1?state=<inputMessage>
  server.on("/room_1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage_2;
    String inputParam_2;
    // GET input1 value on <ESP_IP>/room_1?state=<inputMessage_2>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage_2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam_2 = PARAM_INPUT_2;
      digitalWrite(output_1, inputMessage_2.toInt());
      ledState_1 = !ledState_1;
    }
    else {
      inputMessage_2 = "No message sent";
      inputParam_2 = "none";
    }
    Serial.println(inputMessage_2);
    request->send(200, "text/plain", "OK");
  });  
  
  // Send a GET request to <ESP_IP>/room_2?state=<inputMessage>
  server.on("/room_2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage_2;
    String inputParam_2;
    // GET input1 value on <ESP_IP>/room_2?state=<inputMessage_2>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage_2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam_2 = PARAM_INPUT_2;
      digitalWrite(output_2, inputMessage_2.toInt());
      ledState_2 = !ledState_2;
    }
    else {
      inputMessage_2 = "No message sent";
      inputParam_2 = "none";
    }
    Serial.println(inputMessage_2);
    request->send(200, "text/plain", "OK");
  });
 // Send a GET request to <ESP_IP>/state
  server.on("/state_1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output_1)).c_str());
  });
  // Send a GET request to <ESP_IP>/state
  server.on("/state_2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output_2)).c_str());
  });
  // Start server
  server.begin();
}
  
void loop() {
  // read the state of the switch into a local variable:
  int reading_1 = digitalRead(buttonPin_1); 
  int reading_2 = digitalRead(buttonPin_2); 

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading_1 != lastbuttonState_1) {
    // reset the debouncing timer
    lastDebounceTime_1 = millis();
  }
  if (reading_2 != lastbuttonState_2) {
    // reset the debouncing timer
    lastDebounceTime_2 = millis();
  }
  
  if ((millis() - lastDebounceTime_1) > debounceDelay_1) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading_1 != buttonState_1) {
      buttonState_1 = reading_1;

      // only toggle the LED if the new button state is HIGH
      if (buttonState_1 == HIGH) {
        ledState_1 = !ledState_1;
      }
    }
  }
  if ((millis() - lastDebounceTime_2) > debounceDelay_2) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading_2 != buttonState_2) {
      buttonState_2 = reading_2;

      // only toggle the LED if the new button state is HIGH
      if (buttonState_2 == HIGH) {
        ledState_2 = !ledState_2;
      }
    }
  }

  // set the LED:
  digitalWrite(output_1, ledState_1);
  digitalWrite(output_2, ledState_2);


  // save the reading. Next time through the loop, it'll be the lastbuttonState_2:
  lastbuttonState_1 = reading_1;
  lastbuttonState_2 = reading_2;
}
