// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
// Replace with your network credentials
const char* ssid = "linux";
const char* password = "linuxxxx";
int room1 = 2;
int room2 = 0;
int room3 = 5;

const char* PARAM_INPUT_1 = "room";
const char* PARAM_INPUT_2 = "room_state";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
</html>
)rawliteral";
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);   
  pinMode(room1, OUTPUT);
  pinMode(room2, OUTPUT);
  pinMode(room3, OUTPUT);
  digitalWrite(room1, LOW);
  digitalWrite(room2, LOW);
  digitalWrite(room3, LOW); 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/control?room=<inputMessage1>&room_state=<inputMessage2>
  server.on("/control", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/control?room=<inputMessage1>&room_state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      if ( (inputMessage1.toInt()== 2 ||inputMessage1.toInt()== 1 || inputMessage1.toInt()== 3) &&
      (inputMessage2.toInt() == 1 || inputMessage2.toInt() == 0)){
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
      } else { 
      request->send(200, "text/plain", "Invalid request");
        }
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
      request->send(200, "text/plain", "NOT FOUND");
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}
void loop() {
}
