#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
const int led = 2;
void handleRoot(); // function prototypes for HTTP handlers
void handleLED();
void handleNotFound();

const char *ssid = "TP-LINK_BF4C"; // Enter your WiFi name
const char *password = "1020304050";  // Enter WiFi password

void setup(void){
Serial.begin(115200); // Start the Serial communication to send messages to the computer
delay(10);

Serial.println('\n');
WiFi.begin(ssid, password); // Connect to the network
Serial.print("Connecting to ");
Serial.print(ssid); Serial.println(" ...");
int i = 0;
while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
delay(1000);
Serial.print(++i); Serial.print(' ');
}
Serial.println('\n');
Serial.println("Connection established!");
Serial.print("IP address:\t");
Serial.println(WiFi.localIP());
  
server.on("/", HTTP_GET, handleRoot); // Call the 'handleRoot' function when a client requests URI"/"
server.on("/LED", HTTP_POST, handleLED); // Call the 'handleLED' function when a POST request is made to URI "/LED"
server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
server.begin(); // Actually start the server
Serial.println("HTTP server started");
}

void loop(void){
server.handleClient(); // Listen for HTTP requests from clients
}
void handleRoot() { // When URI / is requested, send a web page with a button to toggle the LED
server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\"value=\"Toggle LED\"></form>");
}
void handleLED() { // If a POST request is made to URI /LED
digitalWrite(led,!digitalRead(led)); // Change the state of the LED
server.sendHeader("Location","/"); // Add a header to respond with a new location for the browser to go to the home page again
server.send(303); // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}
void handleNotFound(){
server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
