#include "Debouncer.hpp"

Debouncer::Debouncer(short pin, unsigned int debounceDelay) {
  _pin = pin;

  _debounceDelay = debounceDelay;
  _lastDebounceTime = millis();

  _state = digitalRead(pin);
  _lastState = _state;
}

bool Debouncer::read() {
  _lastState = _state;
  bool reading = digitalRead(_pin);

  if (reading != _lastRead) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    if (reading != _state) {
      _state = reading;
    }
  }
  _lastRead = reading;
  return _state;
}

bool Debouncer::isFallingEdge() { return !_state && _lastState; }

bool Debouncer::isRisingEdge() { return _state && !_lastState; }
