/**
   Copyright @2020 by Alexzander Purwoko Widiantoro <purwoko908@gmail.com>
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/**
 * This is a module to drive a SIPO registers (Serial In Parallel Out)
 * Which has 1 serial input's with 8 parallel output's. This typically a 74HC595 chips (IC's)
 * or either chips that supports SIPO register's. You can add as many IC as you want by 
 * cascade them, and change the value of constructor with 2, or more by the count of IC cascaded.
 * By defaults, the IC is set into one.
 */

#ifndef __SHIFTREGISTER8OUT__
#define __SHIFTREGISTER8OUT__
#include <Arduino.h>
typedef struct __pinData__ {
  byte _data; // pin Data, must be either LOW or HIGH
  bool _locked;// the current state of pins
  short _owner; // the owner's ID's
} __PinData;

class Shift8RegisterOut {
  public:

    /**
       default constructor
       @param numIC can be customized as many as 74hc595 IC's count.
    */
    Shift8RegisterOut(int _dataPin, int _clockPin, int _latchPin, int numIC = 1) {
      this -> _dataPin = _dataPin;
      this -> _clockPin = _clockPin;
      this -> _latchPin = _latchPin;
      pinMode(_dataPin, OUTPUT);
      pinMode(_clockPin, OUTPUT);
      pinMode(_latchPin, OUTPUT);
      _count = 8 * numIC;
      _data = (__PinData *) malloc(sizeof(__PinData) * _count);
      for (int _x = 0; _x < _count; _x++) {
        __PinData *_d = &_data[_x];
        _d -> _owner = 0;
        _d -> _locked = false;
        _d -> _data = LOW;
      }
    }

    /**
       Destructs the objects.
    */
    ~Shift8RegisterOut() {
      free(_data);
    }

    /**
       Flushing the bytes out by serially into IC's
    */
    void flush();

    /**
       Sets the value of pin numbers on IC's
       allowed if the value on @param reservedBy
       is same as defined on struct
    */
    void set(int numPin, byte data, short owner = 0);

    /**
       Get's the current state of pin's.
       Is locked or not.
    */
    bool isPinLocked(int numPin);

    /**
       Lock's the pin on the specified pins
    */
    void changeStateLockPin(int _numPin, short owner = 0);

    /**
       Get's the current count of pins.
    */
    int count();

    /**
       Reserve the pin with the owner setted.
       So, another user cannot set the value if using different owner
    */
    void reservePin(int startPin, int count, short reservedBy);

    /**
       Gets the count of available pins, that's
       not reserved by another owner.
    */
    int available();

    /**
       Gets the array of available Pins. This value owning some of memory
       So, you must freeing this memory with free() methods
    */
    int *availablePins();

  private:
    int _dataPin;
    int _clockPin;
    int _latchPin;
    __PinData *_data;
    int _count;
    byte lockedPinState = LOW;
    void __set(byte _data, int _numPin, short _owner);
};

#endif
