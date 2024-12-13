//LIBS
#include <WiFi.h>
#include <HTTPClient.h>

//LEDS
#define led_yellow 13 // Pin for yellow led
#define led_green 41 // Pin for green led
#define led_red 40 // Pin for red led

//LDR
const int ldrPin = 4;  // the number of the pushbutton pin
int threshold=600;

//BUTTON
const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed

//DEBOUNCE
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonPressCount = 0;


void setup() {
  //Initial configuration of the pins for led control as OUTPUTS 
  pinMode(led_yellow,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_red,OUTPUT);

  // Inicialization of the inputs
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input

  digitalWrite(led_yellow, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_red, LOW);

  //wifi connection
  Serial.begin(9600);
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void sendHttpRequest() {
  if (WiFi.status() == WL_CONNECTED) {  // If esp connected to the internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint of HTTP requisition

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) { // Result of HTTP Requisition
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {

  int ldrstatus = analogRead(ldrPin);
  buttonState = digitalRead(buttonPin);

  //4
  if (ldrstatus <= threshold) {
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, LOW);
    
    delay(1000);
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, LOW);

    delay(1000);
  }

  //5
  if (ldrstatus >= threshold) {
    //3 seconds green
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, HIGH);
    digitalWrite(led_red, LOW);
    delay(3000);

    //2 seconds yellow
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, LOW);
    delay(2000);

    //5 seconds red
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, LOW);
    digitalWrite(led_red, HIGH);
    delay(5000);
  }

  //6
  //filter out any noise by setting a time buffer
  if ( (millis() - lastDebounceTime) > debounceDelay) {

    if ( (buttonState == HIGH) && (ldrstatus >= threshold) && digitalRead(led_red) == HIGH) {

      delay(1000);
      digitalWrite(led_red, LOW);
      digitalWrite(led_yellow, LOW);
      digitalWrite(led_green, HIGH);
      lastDebounceTime = millis(); //set the current time
    }

    if ( (ldrstatus >= threshold) && digitalRead(led_red) == HIGH) {

      if ( (millis() - lastDebounceTime) > debounceDelay) {
        if (buttonState == HIGH) {
          buttonPressCount++;
          lastDebounceTime = millis();
        }
      }

      //8
      if (buttonPressCount >= 3) {
        sendHttpRequest();
        buttonPressCount = 0;  // reset the counter
      }
    }
  }
}