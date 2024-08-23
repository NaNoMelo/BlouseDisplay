#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <Arduino.h>

class Debouncer {
 public:
  Debouncer(short pin, unsigned int debounceDelay = 50);
  bool read();
  bool getState() { return _state; }
  bool isFallingEdge();
  bool isRisingEdge();

 private:
  unsigned short _pin;
  unsigned int _debounceDelay;
  unsigned long _lastDebounceTime;
  bool _state;
  bool _lastRead;
  bool _lastState;
};

#endif  // DEBOUNCER_H