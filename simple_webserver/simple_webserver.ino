#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

ESP8266WebServer server;

char* ssid = "Tenda";
char* password = "Singhsp+";

void setup() {
  pinMode(2, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "Hello Clients!");
  });
  server.on("/toggle", toggleLED);
  server.begin();
}

void loop() {
  server.handleClient();
}

void toggleLED() {
  digitalWrite(2, !digitalRead(2));
  server.send(204, "");
}

