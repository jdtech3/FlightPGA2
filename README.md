# FlightGPA2
Implementing a flight simulator (term used loosely...) in FPGA on the DE1-SoC: this time with help of Nios II! 

This is a continuation/rewrite of the first version of the project: [FlightGPA](https://github.com/jdtech3/FlightGPA),
where we tried to do the project in Verilog only.

### File organization

  * `fpga/`: Quartus project for running on FPGA: edited version of "DE1-SoC Computer System"
  * `software/`: C/C++ code for running on Nios II core
  * `bsp/`: board support package for Nios II core
  * `raspberrypi/`: Python code for running on Raspberry Pi to provide interface to USB joystick

### Team

* [Joe Dai](https://github.com/jdtech3/)
* [Swarnava Ghosh](https://github.com/swarnavaghosh04/)
