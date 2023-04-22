#include "PinModule.hpp"
class LedModule
{
private:
  PinModule *pin;

public:
  LedModule(AsyncWebServer *server, uint pin, char *endpoint)
  {
    this->pin = new PinModule(pin, 2);

    server->on(endpoint, HTTP_GET,
               [this](AsyncWebServerRequest *request)
               {
                 StaticJsonDocument<200>
                     doc;
                 doc["pinState"] = this->pin->getState();

                 String res;

                 serializeJson(doc, res);
                 request->send(200, "application/json", res);
               });

    server->on(endpoint, HTTP_POST,
               [this](AsyncWebServerRequest *request)
               {
                 this->pin->write(!this->pin->getState());
                 Serial.println(this->pin->getState());

                 StaticJsonDocument<200>
                     doc;
                 doc["pinState"] = this->pin->getState();

                 String res;
                 serializeJson(doc, res);
                 request->send(200, "application/json", res);
               });
  }

  ~LedModule()
  {
    delete this->pin;
  }
};