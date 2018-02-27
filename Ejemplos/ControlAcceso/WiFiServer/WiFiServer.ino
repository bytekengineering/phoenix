//BASED ON CODE FROM "ESP8266 Thing Hookup Guide"
////https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server


#include <ESP8266WiFi.h>
//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "bilbaodynamics";

// Open server on port 80
WiFiServer server(80);

#define TRIGGERPIN D10
#define ECHOPIN    D8

void setup() {
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  client.flush();

  long duration, distance;
  // Clears the trigPin
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  // Sets the trigPin on HIGH state for 12 micro seconds
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12);  
  digitalWrite(TRIGGERPIN, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHOPIN, HIGH);
  // Calculating the distance
  distance = (duration*0.034) / 2;
  
  // Sends data to client
  client.println(distance);
  
}
void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "Phoenix " + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  // Setup Ultrasonic sensor
  pinMode(TRIGGERPIN, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHOPIN, INPUT); // Sets the echoPin as an Input
     
}
