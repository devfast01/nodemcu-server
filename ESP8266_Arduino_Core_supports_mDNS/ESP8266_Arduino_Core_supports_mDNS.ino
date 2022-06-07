#include <ESP8266WiFi.h> // Include the Wi-Fi library
#include <ESP8266mDNS.h> // Include the mDNS library

const char *ssid = "esp8266"; // The name of the Wi-Fi network that will be created
const char *password = "esp_8266"; // The password required to connect to it, leave blank for an

void setup() {
Serial.begin(115200);
delay(10);
Serial.println('\n');
WiFi.softAP(ssid, password); // Start the access point
Serial.print("Access Point \"");
Serial.print(ssid);
Serial.println("\" started");
Serial.print("IP address:\t");
Serial.println(WiFi.softAPIP()); // Send the IP address of the ESP8266 to the computer

if (!MDNS.begin("esp8266")) { // Start the mDNS responder for esp8266.local
Serial.println("Error setting up MDNS responder!");
}
Serial.println("mDNS responder started");


}
void loop() { }
