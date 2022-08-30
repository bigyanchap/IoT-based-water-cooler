/*The wifi related code will be in this file.
 *This file will send (or maybe, receive in future) code to (and from) arduino/atmega328 avr
 *The crucial info like "wifi connected", "or wifi disconnected" will be sent.
 *Author: Bigyan Chapagain, 21 Oct, 2017*/
#include <ESP8266WiFi.h>
//D7 of nodeMCU will be connected to pin10 of Arduino.
//#include <Ticker.h>//Timer in nodeMCU
//Ticker ticker;
const char* ssid = "Linux"; // SSID i.e. Service Set Identifier is the name of your WIFI
const char* password = "0123456789"; // Your Wifi password, in case you have open network comment the whole statement.

int ledPin = 13; // GPIO13 or for NodeMCU you can directly write D7 
WiFiServer server(80); // Creates a server that listens for incoming connections on the specified port, here in this case port is 80.

void setup() {
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  digitalWrite(D1,LOW);//D1 connects to INT0 of arduino (interfaced by logic level shifter.)
  digitalWrite(D2,LOW);//D2 connects to INT1 of arduino (interfaced by logic level shifter.)
  digitalWrite(D3,LOW);
  Serial.begin(115200);
  delay(10);
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  //Trying to connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  digitalWrite(D3,HIGH);
  //D1=HIGH, D0=HIGH, wifi connected
  // Start the server
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
// Wait until the client sends some data
//  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
//  Serial.println(request);
  client.flush();

  
  // Match the request
  boolean inr=false;
  if (request.indexOf("/INR") != -1)  {
    inr=true;
    digitalWrite(D2,HIGH);
    delay(50);//10ms impulse
    digitalWrite(D2,LOW);
  }
  boolean dcr=false;
  if (request.indexOf("/DCR") != -1)  {
    dcr=true;
    digitalWrite(D1,HIGH);
    delay(50);//10ms impulse
    digitalWrite(D1,LOW);   
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<br><br>");
//  client.println("Status: ");
  if(inr)client.println("Temp increment invoked");
  else if(dcr)client.println("Temp decrement invoked");
  else client.println("Press buttons to increase or decrease temperature");
  client.println("<br><br>");
  client.println("<a href=\"/INR\"\"><button>Increase </button></a>");
  client.println("<a href=\"/DCR\"\"><button>Decrease </button></a><br />");  
  client.println("</html>");
  delay(1);
//  Serial.println("Client disconnected");
//  Serial.println("");
}
