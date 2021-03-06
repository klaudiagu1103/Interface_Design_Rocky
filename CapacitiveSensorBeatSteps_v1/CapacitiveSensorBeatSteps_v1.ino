#include <CapacitiveSensor.h>
#include <ESP8266WiFi.h>

WiFiClient client;

/*
   CapitiveSense Library Demo Sketch
   Paul Badger 2008
   Uses a high value resistor e.g. 10M between send pin and receive pin
   Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
   Receive pin is the sensor pin - try different amounts of foil/metal on this pin
*/

// Wifi credentials
const char* ssid     = "Klaudias iPhone";
const char* password = "19971997";
// Server
const char* host = "172.20.10.11";
const int httpPort = 3000;


CapacitiveSensor   cs_unten_rechts = CapacitiveSensor(1, 3);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_unten_links = CapacitiveSensor(1, 15);
CapacitiveSensor   cs_oben_rechts = CapacitiveSensor(1, 13);
CapacitiveSensor   cs_oben_links = CapacitiveSensor(1, 12);
int count = 0;
int ledA = 13;
int ledB = 12;
int ledC = 11;
int ledD = 10;
int ledE = 9;
boolean pressed_unten_rechts = false;
boolean pressed_unten_links = false;
boolean pressed_oben_rechts = false;
boolean pressed_oben_links = false;

void setup()
{
  Serial.begin(9600);
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
  cs_unten_rechts.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  cs_unten_links.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_oben_rechts.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_oben_links.set_CS_AutocaL_Millis(0xFFFFFFFF);

  //pinMode(ledA, OUTPUT);
  //pinMode(ledB, OUTPUT);
  //pinMode(ledC, OUTPUT);
  //pinMode(ledD, OUTPUT);
  //pinMode(ledE, OUTPUT);
}

void loop()
{
  long start = millis();
  long total_unten_rechts = cs_unten_rechts.capacitiveSensor(30);
  long total_unten_links = cs_unten_links.capacitiveSensor(30);
  long total_oben_rechts = cs_oben_rechts.capacitiveSensor(30);
  long total_oben_links = cs_oben_links.capacitiveSensor(30);
  if (capacitorPressed (total_unten_rechts, &pressed_unten_rechts)
      || capacitorPressed (total_unten_links, &pressed_unten_links)
      || capacitorPressed (total_oben_rechts, &pressed_oben_rechts)
      || capacitorPressed (total_oben_links, &pressed_oben_links))
  {
    count++;
    sendRequest(false);
  }

  if (count > 5)
  {
    digitalWrite(ledA, HIGH);
  }
  else
  {
    digitalWrite(ledA, LOW);
  }

  if (count > 7)
  {
    digitalWrite(ledB, HIGH);
  }
  else
  {
    digitalWrite(ledB, LOW);
  }

  if (count > 9)
  {
    digitalWrite(ledC, HIGH);
  }
  else
  {
    digitalWrite(ledC, LOW);
  }

  if (count > 11)
  {
    digitalWrite(ledD, HIGH);
  }
  else
  {
    digitalWrite(ledD, LOW);
  }

  if (count > 13)
  {
    digitalWrite(ledE, HIGH);
  }
  else
  {
    digitalWrite(ledE, LOW);
  }


  Serial.print(total_unten_rechts);
  Serial.print("\t");
  Serial.print(count);
  Serial.print("\t");
  Serial.println(pressed_unten_rechts);

  delay(100);                             // arbitrary delay to limit data to serial port
}

boolean capacitorPressed (long total, boolean *cs_pressed)
{
  if (total > 50)
  {
    if (*cs_pressed == false)
    {
      *cs_pressed = true;
      return true;
    }
  }
  else
  {
    *cs_pressed = false;
  }
  return false;
}

void sendRequest (bool stopSong)
{
  // Use WiFiClient class to connection
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url;

  if (stopSong)
  {
    url = "/stopRocky";
  }
  else {
    url = "/playRocky";

  }

  Serial.print("Requesting URL: ");
  Serial.println(url);
  // Send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
}
