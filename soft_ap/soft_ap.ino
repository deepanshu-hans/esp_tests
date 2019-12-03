#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

ESP8266WebServer server;

char* ssid = "Tenda";
char* password = "Singhsp+";
char* espssid = "warrior";
char* esppass = "within82";

IPAddress ip(192,168,11,2);
IPAddress gateway(192,168,11,1);
IPAddress subnet(255,255,255,0);

void setup() {
  pinMode(2, OUTPUT);
  WiFi.mode(WIFI_AP_STA);
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
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(espssid, esppass);
}

void loop() {
  server.handleClient();
}

void toggleLED() {
  digitalWrite(2, !digitalRead(2));
  server.send(204, "");
}

