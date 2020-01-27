#include <ESP8266WiFi.h>
WiFiClient client;
// ===================================================
const char* ssid     = "Klaudias iPhone";
const char* password = "19971997";

const char* host = "172.20.10.11";
const int httpPort = 3000;

const int buttonPin = 4;     // pin number of button 
const int ledPin = 5;     // pin number of led 

// initialize buttonState 
int buttonState = 0;

// Holds previous state of button --> off / on 
// Helps to prevent "spaming" of button  
String previousButtonState = "off";

void setup() {
  // Setup serial monitor to 9600 baud
  Serial.begin(9600);
  delay(10);

  // Start connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); //works!

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // initialize the pushbutton pin as an input, ledPin as output
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.println("Setup comleted");

}

void loop() {
  // Read state of button
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    if(previousButtonState == "off"){
      Serial.println("button pressed");
      digitalWrite(ledPin, HIGH);
      
      // Use WiFiClient class to connection
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      
      String url = "/playRocky";
      
      Serial.print("Requesting URL: ");
      Serial.println(url);
      // Send the request to the server
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    }
    previousButtonState = "on";
  }

  if (buttonState == LOW){
    digitalWrite(ledPin, LOW);
    previousButtonState = "off";

  }
  
 }
