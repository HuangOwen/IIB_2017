# IIB_2017

## Dependency
OpenCV 2.4



## Project Structure
- `main.cpp`: master
- `trackInit.h`,`trackInit.cpp`: generate the map and determine the turning positions
- `traceCar.h`,`traceCar.cpp`: recognize the position of the car
- `controlCar.h`,`controlCar.cpp`: send the bluetooth instruction according to the car position.
- `BLE_CAR.h`,`BLE_PORT.h`: support for bluetooth communication
- `board.ino`: the code on  MSP430 board 
