#include "Shift8RegisterOut.h"

void Shift8RegisterOut::__set(byte _data, int _numPin, short _owner) {
  if (_numPin < 0 || _numPin >= _count || isPinLocked(_numPin)) return;
  __PinData *_d = &(this -> _data)[(_count-1) - _numPin];
  if (_d -> _owner != _owner)return;
  _d -> _data = _data;
}

void Shift8RegisterOut::set(int numPin, byte data, short owner) {
  __set(data, numPin, owner);
}

void Shift8RegisterOut::changeStateLockPin(int _numPin, short owner) {
  if (_numPin < 0 || _numPin >= _count) return;
  int actualPin = (_count-1) - _numPin;
  if (_data[actualPin]._owner != owner) return;
  _data[actualPin]._locked = !_data[actualPin]._locked;
}

void Shift8RegisterOut::reservePin(int startPin, int count, short owner) {
  if (startPin < 0 || startPin >= _count || (startPin + count) > _count) return;
  for (int _x = startPin; _x < count; _x++) {
    __PinData *_d = &(this -> _data)[(_count-1) - _x];
    if (_d -> _owner == 0) _d -> _owner = owner;
  }
}

int Shift8RegisterOut::available(){
  int count = 0;
  for(int _x = 0; _x < _count; _x++){
    if(_data[_x]._owner == 0) count++;
  }
  return count;
}

int *Shift8RegisterOut::availablePins(){
  int count = available();
  int *arr = (int *) malloc(sizeof(int) *count);
  for(int _x = _count - 1, _y = 0; _x >= 0; _x--){
    if(_data[_x]._owner == 0) arr[_y++] = (_count - 1) - _x;
  }
}

void Shift8RegisterOut::flush() {
  digitalWrite(_latchPin, LOW);
  for (int _x = 0; _x < _count; _x++) {
    __PinData *_d = &_data[_x];
    digitalWrite(_dataPin, _d -> _data);
    digitalWrite(_clockPin, HIGH);
    digitalWrite(_clockPin, LOW);
  }
  digitalWrite(_latchPin, HIGH);
  //delayMicroseconds(1);
}

bool Shift8RegisterOut::isPinLocked(int _numPin) {
  if (_numPin < 0 || _numPin >= _count) return false;
  return _data[(_count-1) - _numPin]._locked;
}

int Shift8RegisterOut::count() {
  return _count;
}
