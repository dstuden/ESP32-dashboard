#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "DHT.h"
#include "EEPROM.h"

#include "modules/Modules.hpp"
#define EEPROM_SIZE 4096

struct Config
{
  char *SSID;
  char *KEY;
} cfg;

bool autoMode = false;

AsyncWebServer server(80);

LedModule *led1;
LedModule *led2;
DHTModule *dht;

void setupWifi()
{
  cfg.SSID = "This pppphone";
  cfg.KEY = "zululvivon1";

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
  led1 = new LedModule(&server, 5, "/heater");
  led2 = new LedModule(&server, 16, "/humidifier");
  dht = new DHTModule(&server, 17, "/dht");

  server.on("/auto", HTTP_GET,
            [](AsyncWebServerRequest *request)
            {
              StaticJsonDocument<200>
                  doc;
              doc["state"] = autoMode;

              String res;

              serializeJson(doc, res);
              request->send(200, "application/json", res);
            });

  server.on("/auto", HTTP_POST,
            [](AsyncWebServerRequest *request)
            {
              autoMode = !autoMode;

              StaticJsonDocument<200>
                  doc;
              doc["state"] = autoMode;

              String res;
              serializeJson(doc, res);
              request->send(200, "application/json", res);
            });

  server.on("/setTemp", HTTP_POST,
            [](AsyncWebServerRequest *request)
            {
              int hum = request->arg("temp").toInt();
              EEPROM.write(2, 30);
              EEPROM.commit();

              request->send(200);
            });
  server.on("/setHum", HTTP_POST,
            [](AsyncWebServerRequest *request)
            {
              int hum = request->arg("hum").toInt();
              EEPROM.write(3, hum);
              EEPROM.commit();

              request->send(200);
            });
}

void setup()
{
  if (!SPIFFS.begin(true, "/data"))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println("start...");
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM");
    delay(1000);
  }

  EEPROM.write(0, 2); // heater
  EEPROM.commit();
  EEPROM.write(1, 2); // humidifier
  EEPROM.commit();

  setupWifi();
  setupServer();
  setupModules();

  Serial.println();
  Serial.print("You can connect to ");
  Serial.println(WiFi.localIP());
}

int oldMillis = 0;

void loop()
{
  int currentMillis = millis();
  if (currentMillis - oldMillis >= 1000)
  {
    oldMillis = currentMillis;

    // turn heater and humidifier on/off depending on temperature and
    // humidity and hysteresis saved in eeprom
    int heater = EEPROM.read(0) || 2;
    int humidifier = EEPROM.read(1) || 2;
    int wanted_temp = EEPROM.read(2) || 20;
    int wanted_hum = EEPROM.read(3) || 50;

    Serial.println("heater: " + heater);
    Serial.println("humidifier: " + humidifier);
    Serial.println("wanted_temp: " + wanted_temp);
    Serial.println("wanted_hum: " + wanted_hum);

    if (autoMode)
    {
      if (dht->temperature <= 30 - heater)
        led1->pin->write(1);
      if (dht->temperature >= 30 + heater)
        led1->pin->write(0);
      if (dht->humidity <= wanted_hum - humidifier)
        led2->pin->write(1);
      if (dht->humidity >= wanted_hum + humidifier)
        led2->pin->write(0);
    }
  }
}
