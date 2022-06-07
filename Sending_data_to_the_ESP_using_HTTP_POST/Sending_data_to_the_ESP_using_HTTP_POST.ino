#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80

void handleRoot(); // function prototypes for HTTP handlers
void handleLogin();
void handleNotFound();
const char* ssid = "linux"; // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "linux123"; // The password of the Wi-Fi 

void setup(void) {
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

  server.on("/", HTTP_GET, handleRoot); // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/login", HTTP_POST, handleLogin); // Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.onNotFound(handleNotFound); // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin(); // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient(); // Listen for HTTP requests from clients
}
void handleRoot() { // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/login\" method=\"POST\"><input type=\"text\"name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\"placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'John Doe' and 'password123' ...</p>");
}
void handleLogin() { // If a POST request is made to URI /login
  if ( ! server.hasArg("username") || ! server.hasArg("password")|| server.arg("username") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request"); // The request is invalid, so send HTTP status 400
    return;
  }
    if(server.arg("username") == "Devfast" && server.arg("password") == "smartiot_123") { // If both the username and the password are correct
    server.send(200, "text/html", "<h1>Welcome, " + server.arg("username") + "!</h1><p>Login successful</p>");
  } else { // Username and password don't match
    server.send(401, "text/plain", "401: Unauthorized");
  }
}
void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
