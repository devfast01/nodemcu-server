#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h> // Include the WebServer library

ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
void handleRoot(); // function prototypes for HTTP handlers
void handleesp();
void handleNotFound();

const char* ssid = "linux"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "linux123"; // The password of the Wi-Fi network


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
Serial.println(WiFi.localIP()); // Send the IP address of the ESP8266 to the computer
server.on("/", handleRoot); // Call the 'handleRoot' function when a client requests URI"/"
server.on("/esp", handleesp);
server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
server.begin(); // Actually start the server
Serial.println("HTTP server started");
}
void loop(void){
server.handleClient(); // Listen for HTTP requests from clients
}
void handleRoot() {
server.send(200, "text/plain", "Hello world!"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}
void handleesp() {
server.send(200, "text/plain", "Esp8266 !"); // Send HTTP status 200 (Ok) and send some text to the browser/client
}
void handleNotFound(){
server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
