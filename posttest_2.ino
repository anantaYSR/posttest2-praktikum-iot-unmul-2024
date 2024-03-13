#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const int LED = D2;
const int BUZZER = D0;
const int BTN_ENABLED = D5;
const int BTN_UP = D6;
const int BTN_MIN = D7;

int times = 0;
int lightValue = 0;
int buzzerValue = 0;
int enabledComponent = 1;

// Access point credentials
const char* ap_ssid = "ESP8266_AP";
const char* ap_password = "password";

// Web server
ESP8266WebServer server(80);

void handleRoot() {
  String html = "<h1>Hello from LiDAR</h1>";
  html += "<p>Value Lampu:";
  html += String(lightValue);
  html += "</p>";
  html += "<p>Value  Buzzer:";
  html += String(buzzerValue);
  html += "</p>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN_ENABLED, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_MIN, INPUT_PULLUP);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("Access point started");
  Serial.print("SSID: ");
  Serial.println(ap_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Start web server
  server.on("/", handleRoot);
  server.begin();

  // SETUP
  noTone(BUZZER);
  digitalWrite(LED, LOW);
}


void loop() {
  server.handleClient();
  delay(100);
  enabledComponent = digitalRead(BTN_ENABLED);
  int btnUpState = digitalRead(BTN_UP);
  int btnMinState = digitalRead(BTN_MIN);
  
  if(enabledComponent == 0) {
    analogWrite(LED, lightValue);
    tone(BUZZER, buzzerValue);
  } else {
    analogWrite(LED, 0);
    noTone(BUZZER);
  }

  if(btnUpState == 0) {
    times = times + 1;
    lightValue = lightValue < 249 ? 3 * times : lightValue;
    buzzerValue = buzzerValue < 2500 ? 10 * times : buzzerValue;
  }

  if(btnMinState == 0) {
    times = times == 0 ? 0 : times - 1;
    lightValue = lightValue > 0 ? 3 * times : 0;
    buzzerValue = buzzerValue > 0 ? 10 * times : 0;
  }
}
