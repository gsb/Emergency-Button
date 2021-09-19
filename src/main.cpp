
#define INFO "433MHz Button 0.1.0"

//-- Main Cofiguration ----------------------------------------------------------

#include <Arduino.h>

#define YOUR_SSID  "..."
#define YOUR_PASS  "..."
#define YOUR_EVENT "..."
#define YOUR_KEY   "..."


#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
  #define BUTTON_INPUT_PIN 34 // 433MHz Switch Input Pin
  #define BUTTON_INPUT_VALUE 4095
#elif defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
  #define BUTTON_INPUT_PIN 0  // 433MHz Switch Input Pin
  #define BUTTON_INPUT_VALUE 1024
#else
  // System not supported.
#endif


//-- Globals, Prototypes and such...
char trigger[96];  // sizeof: "/trigger//with/key/" + YOUR_EVENT + YOUR_KEY +1
WiFiClient client; // WiFi Client for ESP's HTTP GET request to IFTTT.
const char* host = "maker.ifttt.com";
int buttonState = LOW;    // initialize button state as OFF.
uint64_t buttonStartTime; // used for click down duration.


//-- Support Methods ------------------------------------------------------------

void getResponse() {
  while (client.connected()) // Clear IFTTT response headers via read and skip.
    if ((client.readStringUntil('\n'))=="\r") break; // Done with headers.
  String reply = client.readStringUntil('\n'); // Read 1-line text response.
  Serial.printf("Reply << %s\n", reply.c_str());
}

void handleClickEvent() {
  Serial.printf("Connecting to %s\n", host);
  if (!client.connect(host, 80)) {
    Serial.println("Connection failed!");
  } else {
    Serial.printf("Requesting: %s\n", trigger);
    client.print(String("GET ") + trigger + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "User-Agent: BuildFailureDetectorESP8266\r\n" +
        "Connection: close\r\n\r\n");
    getResponse(); //...optional.
  }
}

int readSignal() {
  for (int samples=0; analogRead(BUTTON_INPUT_PIN) == BUTTON_INPUT_VALUE; samples++) {
    delay(10); // Pause and then, check again...
    if (samples == 2) return HIGH; // Sequence of HIGHs, must be HIGH.
  }
  return LOW;  // Not a sequence of HIGHs, must be LOW.
}

void checkButtonState() { //-- Check Button State and handle button click, if and when.
  int currentState = readSignal();

  if (currentState == HIGH && buttonState == HIGH) return; //...continuing button press.

  if (currentState == HIGH && buttonState == LOW) { // Pin changed to HIGH - ON.}
    digitalWrite(LED_BUILTIN, LOW); // LED_BUILTIN to ON - inverted states.
    Serial.println("button down");
    buttonState = HIGH;
    buttonStartTime = millis();
  }

  if (currentState == LOW && buttonState == HIGH) { // Pin changed to LOW - OFF.
    digitalWrite(LED_BUILTIN, HIGH); // LED_BUILTIN to OFF - inverted states.
    Serial.println("button up");
    buttonState = LOW;
    if (millis() - buttonStartTime > 300) { //...not an accedental bump.
      Serial.println("~~ Alert Button Clicked!");
      handleClickEvent();
    }
  }
}


//-- Arduino Core Methods -------------------------------------------------------

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  WiFi.begin(YOUR_SSID, YOUR_PASS);
  Serial.print("\n\nConnecting to ");
  Serial.print(YOUR_SSID);
  while (WiFi.status() != WL_CONNECTED) {delay(500); Serial.print(".");}
  Serial.println("...connected.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(INFO); // optional

  // Initialize the constant trigger event request.
  snprintf(trigger,sizeof(trigger), "/trigger/%s/with/key/%s", YOUR_EVENT, YOUR_KEY);

  pinMode(LED_BUILTIN, OUTPUT);   // Setup visual display of button state.
  digitalWrite(LED_BUILTIN,HIGH); // Inverted states: HIGH is off, LOW is on.

  Serial.println("\nSetup complete.\n");
}


void loop() {
  checkButtonState();
}

//-------------------------------------------------------------------------------