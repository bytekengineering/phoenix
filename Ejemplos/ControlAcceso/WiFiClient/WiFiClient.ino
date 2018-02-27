/*
 *  This sketch receives data via HTTP GET requests from an other Phoenix. And based on that data 
 *
 */

#include <ESP8266WiFi.h>

// SSID and Password of the WiFi Netword
const char* ssid     = "Phoenix B918";
const char* password = "bilbaodynamics";

const char* host = "192.168.4.1";

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library
#include <Adafruit_NeoPixel.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 6

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, D9, NEO_GRB + NEO_KHZ800);


void setup() {
  // We start by connecting to the Phoenix WiFi network
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

 // Start the NeoPixels
   pixels.begin();
}


void loop() {
  delay(200);
    
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }
  
  // We now create a URI for the request
  String url = "192.168.4.1";

  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server
  if(client.available()){
    String line = client.readStringUntil('\r');
    
    // Convert the String to long
    long dist = line.toInt();

    
    if(dist>6) {
      for(int i=0;i<NUMPIXELS;i++)
      {
       pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
      } 
     pixels.show(); // This sends the updated pixel color to the hardware.  
    }
    else {
      for(int i=0;i<NUMPIXELS;i++)
      {
       pixels.setPixelColor(i, pixels.Color(150,0,0)); // Moderately bright red color.
      }
    pixels.show(); // This sends the updated pixel color to the hardware.
    }
  }
  
}

