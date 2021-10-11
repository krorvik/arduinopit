ESPRotary
==========

* Author: Lennart Hennigs (https://www.lennarthennigs.de)
* Copyright (C) 2017-2021 Lennart Hennigs.
* Released under the MIT license.

Arduino Library to simplify reading rotary encoder data.
Tested with Arduino, ESP8266 and ESP32.

Description
-----------

This library allows you read out interactions with a rotary encoder and act on them. 
It uses callback functions to be notified when the rotary encoder changes.

Some of the code based of this library is based on code from [PJRC](https://www.pjrc.com/teensy/td_libs_Encoder.html).


How to Use
----------

__Definition__
- Use the `begin();` or the parameterized constructor to create a new instance of the class
- Encoder produce different numbers of "click" on a single turn. 
- You can specify the number of clicks in the constructor, or via a setter function
    - ```void setStepsPerClick(int steps);```
    - ```int getStepsPerClick();```


__Callback Handlers__
- The library provides several callback handlers to detect events
    - ```void setChangedHandler(CallbackFunction f);```
    - ```void setRightRotationHandler(CallbackFunction f);```
    - ```void setLeftRotationHandler(CallbackFunction f);```
    - ```void setUpperOverflowHandler(CallbackFunction f);```
    - ```void setLowerOverflowHandler(CallbackFunction f);```
- The library does not detect button clicks. You have to use a seperate library for this, e.g. [Button2](https://github.com/LennartHennigs/Button2).


__Ranges__
- In the constructor you can define an upper and a lower treshhold. The encoder value will not be bypass  these values.
- There are also getter and setter functions the these values
    - ```void setUpperBound(int upper_bound);```
    - ```void setLowerBound(int lower_bound);```
    - ```int getUpperBound();```
    - ```int getLowerBound();```


__Reading out information__
- The class allows you the get the position and the direction after a click using these function:
    - ```int getPosition();```
    - ```byte getDirection();```
    - ```String directionToString(byte direction);```


__Speed__
- You can define the speed, i.e. the increment the a single click in the constructor
- There is also a getter and setter function for this
    - ```void setIncrement(int inc);```
    - ```int getIncrement();```


__The Loop__
- For the class to work, you need to call the `loop()` member function in your sketch's `loop()` function.
- See the examples for more details.


Notes
-----

- To see the latest changes to the library please take a look at the [Changelog](https://github.com/LennartHennigs/ESPRotary/blob/master/CHANGELOG.md).
 
- And if you find this library helpful, please consider giving it a star at [GitHub](https://github.com/LennartHennigs/ESPRotary). Thanks!



Class Definition
----------------

These are the constructor and the member functions the library provides:

```
    ESPRotary();
    ESPRotary(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int inital_pos = 0, int increment = 1);

    void begin(byte pin1, byte pin2, byte steps_per_click = 1, int lower_bound = INT16_MIN, int upper_bound = INT16_MAX, int inital_pos = 0, int increment = 1);

    int getPosition();
    void resetPosition(int p = 0, bool fireCallback = true);

    byte getDirection();
    String directionToString(byte direction);

    void setIncrement(int inc);
    int getIncrement();

    void setUpperBound(int upper_bound);
    void setLowerBound(int lower_bound);
    int getUpperBound();
    int getLowerBound();

    void setStepsPerClick(int steps);
    int getStepsPerClick();

    void setChangedHandler(CallbackFunction f);
    void setRightRotationHandler(CallbackFunction f);
    void setLeftRotationHandler(CallbackFunction f);
    void setUpperOverflowHandler(CallbackFunction f);
    void setLowerOverflowHandler(CallbackFunction f);

    void loop();
```

Installation
------------
Open the Arduino IDE choose "Sketch > Include Library" and search for "ESP Rotary".
Or download the ZIP archive (https://github.com/lennarthennigs/ESPRotary/zipball/master), and choose "Sketch > Include Library > Add .ZIP Library..." and select the downloaded file.


License
-------

MIT License

Copyright (c) 2017-2020 Lennart Hennigs

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
