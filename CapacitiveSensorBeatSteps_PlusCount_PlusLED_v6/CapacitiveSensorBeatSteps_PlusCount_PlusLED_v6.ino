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
//Timer setup, trackt wie lang nicht gedrückt wurde
unsigned long timeSteppedOn = 0; // the time the delay started
bool steppedOnTimerRunning = false; 
bool songRunning = false; // Läuft der song?


// Pins Step 1 - sensor HIER NOCHMAL ÄNDERN????
const int step1In = 5; // D1
const int step1Out = 4; // D2
CapacitiveSensor cs_unten_rechts = CapacitiveSensor(step1In, step1Out);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foilCapacitiveSensor   cs_4_2 = CapacitiveSensor(4,5);        // 10 megohm resistor between pins 4 & 2, pin 2 is sensor pin, add wire, foil
CapacitiveSensor   cs_unten_links = CapacitiveSensor(2,4);
CapacitiveSensor   cs_oben_rechts = CapacitiveSensor(2,5);
CapacitiveSensor   cs_oben_links = CapacitiveSensor(2,6);

// setup counter
int count = 0;
int previousCount = 0;

// LED setup
int ledA = 13;
int ledB = 12;
int ledC = 11;
int ledD = 10;
int ledE = 9;

boolean pressed_unten_rechts = false;
boolean pressed_unten_links = false;
boolean pressed_oben_rechts = false;
boolean pressed_oben_links = false;


// Methode, die immer am Anfang aufgerufen wird
void setup()
{
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

  // Autocalibration
  cs_unten_rechts.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  cs_unten_links.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_oben_rechts.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_oben_links.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
  pinMode(ledA,OUTPUT);
  pinMode(ledB,OUTPUT);
  pinMode(ledC,OUTPUT);
  pinMode(ledD,OUTPUT);
  pinMode(ledE,OUTPUT);
}

// Loop Funktion, nach setup aufgerufen
void loop() {
  long start = millis(); // Start wird auf den Zeitpunkt gesetzt, Millis gibt immer die Sekunden an, wie lange das Programm schon läuft
  long total_unten_rechts = cs_unten_rechts.capacitiveSensor(30); // Liest den Sensor aus
  long total_unten_links = cs_unten_links.capacitiveSensor(30);
  long total_oben_rechts = cs_oben_rechts.capacitiveSensor(30);
  long total_oben_links = cs_oben_links.capacitiveSensor(30);
  if (capacitorPressed (total_unten_rechts, &pressed_unten_rechts)
       ||capacitorPressed (total_unten_links, &pressed_unten_links)
       ||capacitorPressed (total_oben_rechts, &pressed_oben_rechts)
       ||capacitorPressed (total_oben_links, &pressed_oben_links)) // Methode wird aufgerufen für Übergabe von total unten rechts und pressed unten rechts, wenn die funktion true zurückgibt ,erhöhe den counter
  {
    count++;
  }

  if (count > 5)
  {
    digitalWrite(ledA, HIGH); // LED
  }
  else
  {
    digitalWrite(ledA, LOW);
  }

   if (count>7)
    {
      digitalWrite(ledB,HIGH);
    }
    else
    {
      digitalWrite(ledB,LOW);
    }

    if (count>9)
    {
      digitalWrite(ledC,HIGH);
    }
    else
    {
      digitalWrite(ledC,LOW);
    }

    if (count>11)
    {
      digitalWrite(ledD,HIGH);
    }
    else
    {
      digitalWrite(ledD,LOW);
    }

    if (count>13)
    {
      digitalWrite(ledE,HIGH);
    }
    else
    {
      digitalWrite(ledE,LOW);
    }


  Serial.print("Count"); // Ausgabe im Monitor
  Serial.print("\t");
  Serial.print(count);
  Serial.print("\t");

  // wenn der button gedrückt ist
  if (pressed_unten_rechts)
  {

    // Wenn der Song noch nicht läuft und der State des Sensors sich verändert hat, starte den Song
    if (songRunning == false && previousCount < count)
    {
      
      sendRequest(false);
      songRunning = true;
    }
  }

  // Wenn der Song läuft und der Sensor seit 1 sekunde nicht gedrückt wurde, stoppe den Song
  if ((songRunning == true && steppedOnTimerRunning && (millis() - timeSteppedOn) >= 500))
  {
    sendRequest(true);
    songRunning = false;
    steppedOnTimerRunning = false;
  }

  previousCount = count;
  delay(100);                             // arbitrary delay to limit data to serial port
}

boolean capacitorPressed (long total, boolean *cs_pressed) // Methode die aufgerufen wird um zu prüfen, ob der Sensor gedrückt wurde oder nicht. Wenn total über 100 ist, dann wurde gedrückt
{
  if (total > 100)
  {
    if (*cs_pressed == false) // Ist der Sensor schon gedrückt? Wenn er schon gedrückt ist, dann gibt nicht zurück, dass er den Count um 1 erhöhen soll (Wenn ich die ganze Zeit halte, dann wird der Count nicht erhöht
    {
     
      *cs_pressed = true;
      return true;
    }
    else 
    {
      timeSteppedOn = millis(); // Wenn ich "frisch" draufdrücke, und vorher noch nicht draufgedrückt wurde, dann starte den Timer
      steppedOnTimerRunning = true;
    }
  }
  else
  {
    *cs_pressed = false; // wenn der nicht über 100 ist, returne false
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
