A C++ state machine that reads input from GPIO pins on a raspberry pi.
It then indicates whether it was a keydown or keyup event.

For hardware I am using a momentary push button switch with a 10k pulldown resistor.
Basic debouncing is done in software.


Includes a basic example of using CMake to refresh my memory.

To build using CMake:
```bash
$ cmake .
$ make
```

To build regulary for now:
```bash
$ g++ -o GPIOButtons GPIOClass.cpp GPIOButtons.cpp -I.
```
