#define DHTTYPE DHT22

class DHTModule
{
private:
  uint pin;
  DHT *dht;
  float humidity = 0.0f;
  float temperature = 0.0f;

public:
  DHTModule(AsyncWebServer *server, uint pin, char *endpoint)
  {

    this->pin = pin;
    this->dht = new DHT(pin, DHTTYPE);

    this->dht->begin();

    server->on(endpoint, HTTP_GET,
               [this](AsyncWebServerRequest *request)
               {
                 this->humidity = this->dht->readHumidity();
                 this->temperature = this->dht->readTemperature();

                 StaticJsonDocument<200>
                     doc;
                 doc["humidity"] = this->humidity;
                 doc["temperature"] = this->temperature;

                 String res;
                 serializeJson(doc, res);
                 request->send(200, "application/json", res);
               });
  }

  ~DHTModule()
  {
  }
};