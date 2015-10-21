A C++ state machine that reads input from GPIO pins on a raspberry pi.
It then indicates whether it was a keydown or keyup event.


Includes a basic example of using CMake to refresh my memory.

To build using CMake:
```bash
$ cmake .
$ make
```

To build regulary for now:
```bash
$ g++ -o GPIOTest2 GPIOClass.cpp GPIOTest2.cpp -I.
```
