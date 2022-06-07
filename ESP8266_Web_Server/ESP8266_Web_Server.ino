#include <ESP8266WiFi.h>

const char *ssid = "TP-LINK_BF4C"; // Enter your WiFi name
const char *password = "1020304050";  // Enter WiFi password

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String room5 = "off";
String room2 = "off";
String room4 = "off";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output2 = 2;
const int output4 = 4;



void setup() {
  Serial.begin(115200);

  pinMode(output5, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output4, OUTPUT);
  digitalWrite(output5, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output4, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) { // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /room4/on") >= 0) {
              room4 = "on";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /room4/off") >= 0) {
              room4 = "off";
              digitalWrite(output4, LOW);
            }
            if (header.indexOf("GET /room5/on") >= 0) {
              room5 = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /room5/off") >= 0) {
              room5 = "off";
              digitalWrite(output5, LOW);
            } else if (header.indexOf("GET /room2/on") >= 0) {
              room2 = "on";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /room2/off") >= 0) {
              room2 = "off";
              digitalWrite(output2, LOW);
            }
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");

            // Display current state, and ON/OFF buttons for GPIO 5
            client.println("<p>ROOM 5 - State " + room5 + "</p>");
            // If the room5 is off, it displays the ON button
            if (room5 == "off") {
              client.println("<p><a href=\"/room5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/room5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<p>ROOM 2 - State " + room2 + "</p>");
            // If the room2 is off, it displays the ON button
            if (room2 == "off") {
              client.println("<p><a href=\"/room2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/room2/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            // Display current state, and ON/OFF buttons for GPIO 2
            client.println("<p>ROOM 4 - State " + room4 + "</p>");
             // If the room4 is off, it displays the ON button
            if (room4 == "off") {
              client.println("<p><a href=\"/room4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/room4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
