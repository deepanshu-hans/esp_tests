#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<ArduinoJson.h>

ESP8266WebServer server;
WiFiClient client;

char* ssid = "Tenda";
char* password = "Singhsp+";

#define WU_API_KEY "3efa9a23f81ddd45"
#define WU_LOCATION "India/Surat"
#define WU_URL "api.wunderground.com"

static char respBuffer[4096];

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
  server.on("/getData", getData);
  server.begin();
}

void loop() {
  server.handleClient();
}

void toggleLED() {
  digitalWrite(2, !digitalRead(2));
  server.send(204, "");
}

void getData() {
  const char request[] =
    "GET /api/" WU_API_KEY "/conditions/q/" WU_LOCATION ".json HTTP/1.1\r\n"
    "User-Agent: ESP8266/0.1\r\n"
    "Accept: */*\r\n"
    "Host: " WU_URL "\r\n"
    "Connection: close\r\n"
    "\r\n";
  Serial.println(request);
  client.connect(WU_URL, 80);
  client.print(request);
  client.flush();
  delay(1000);

  uint16_t index = 0;
  while (client.connected()) {
    if (client.available()) {
      respBuffer[index++] = client.read();
      delay(1);
    }
  }
  client.stop();

  char * json = strchr(respBuffer, '{');
  DynamicJsonBuffer jBuffer;

  JsonObject& root = jBuffer.parseObject(json);
  JsonObject& current = root["current_observation"];
  String temp_c = current["temp_c"];
  String weather = current["weather"];

  String msg = "Temp: " + temp_c + ", Weather: " + weather;
  server.send(200, "text/plain", msg);
}



