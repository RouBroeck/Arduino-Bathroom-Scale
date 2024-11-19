#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define calibration_factor -21400.0

#define LOADCELL_DOUT_PIN  D0
#define LOADCELL_SCK_PIN  D5
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define NET_SSID     = "ExampleNetwork";
#define NET_PASSWD   = "ChangeMe";

HX711 scale;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void print_text(char* msg) {
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 8);
  display.println(msg);
  display.display();
}

void setup_cells() {
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); 
  scale.tare(); //reset offset
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void setup_wifi() {
  WiFi.begin(NET_SSID, NET_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.serveStatic("/", LittleFS, "/");
  server.begin();
}

void setup() {
  Serial.begin(115200);
  Serial.println("starte HX711 scale...");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); //Stop
  }

  delay(10);
  setup_cells();
  setup_wifi();

  Serial.println("...finished");
  delay(1000);
}


void loop() {
  float weight = scale.get_units(5); //take 5 meassurements
  Serial.print("Reading: ");
  Serial.print(weight, 3);
  Serial.print(" kg");
  Serial.println();

  char display_msg [20];
  sprintf(display_msg, "%.1f kg", weight);

  print_text(display_msg);
  ws.textAll(display_msg);
  ws.cleanupClients();
}