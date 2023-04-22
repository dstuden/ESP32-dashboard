class PinModule
{
private:
  uint pin;
  bool mode;
  bool state;

public:
  PinModule(uint pin, uint mode)
  {
    this->pin = pin;
    this->mode = mode;
    this->state = 0;

    pinMode(this->pin, mode);
    digitalWrite(this->pin, this->state);
  }

  bool getState()
  {
    return this->state;
  }

  void write(bool state)
  {
    digitalWrite(this->pin, state);
    this->state = state;
  }

  void setMode(bool mode)
  {
    pinMode(this->pin, mode);
  }
};