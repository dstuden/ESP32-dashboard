#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "DHT.h"

#include "modules/Modules.hpp"

struct Config
{
  char *SSID;
  char *KEY;
  char *hash;
} cfg;

AsyncWebServer server(80);

void setupWifi()
{
  cfg.SSID = "Not a Honeypot";
  cfg.KEY = "zululvivon1";
  cfg.hash = "75c7130e8b0eae5e17392063842ae634365195582766e3b49e10d2f0676db14a";

  Serial.begin(115200);
  WiFi.begin(cfg.SSID, cfg.KEY);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

void setupServer()
{
  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "(: Wrong route!"); });

  server.serveStatic("/", SPIFFS, "/w").setDefaultFile("index.html");

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404);
  } });

  server.begin();
}

void setupModules()
{
  LedModule *led1 = new LedModule(&server, 5, "/led1");
  LedModule *led2 = new LedModule(&server, 16, "/led2");
  DHTModule *dht = new DHTModule(&server, 17, "/dht");
}

void setup()
{
  if (!SPIFFS.begin(true, "/data"))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  setupWifi();
  setupServer();
  setupModules();

  Serial.println();
  Serial.print("You can connect to ");
  Serial.println(WiFi.localIP());
}

void loop()
{
}
