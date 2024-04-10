# FlightGPA2
Implementing a flight simulator (term used loosely...) in FPGA on the DE1-SoC: this time with help of Nios II! 

This is a continuation/rewrite of the first version of the project: [FlightGPA](https://github.com/jdtech3/FlightGPA),
where we tried to do the project in Verilog only.

<img src="https://github.com/jdtech3/FlightGPA2/assets/8998191/5d7bf9bb-779a-4e93-9488-3969b0ccf37e" width="60%">

### Dependencies/Platform

Big thanks to [glm](https://github.com/g-truc/glm) (OpenGL Mathematics library) licensed under the MIT license, without which this project would not be possible.

Targeted for/tested using:
  * Quartus Prime 18.1
  * Python 3.10.2
  * C99 + C++14 standards
  * `glm` 1.0.2 commit [`ab913bbdd0bd10462114a17bcb65cf5a368c1f32`](https://github.com/g-truc/glm/tree/ab913bbdd0bd10462114a17bcb65cf5a368c1f32)
  * Ubuntu 22.04

### File organization

  * `fpga/`: Quartus project for running on FPGA: edited version of "DE1-SoC Computer System"
  * `software/`: C/C++ code for running on Nios II core
  * `bsp/`: board support package for Nios II core
  * `host/`: Python code and data on the host computer, i.e. to provide interface to USB joystick

### Patches required

  * `glm` needs to be patched using `glm_PR1264_fix_uninitialized_in_constexpr_warning.patch` taken from [unmerged PR](https://github.com/g-truc/glm/pull/1264)
  * `nios2eds/bin/gnu/nios2-elf/include/c++/5.3.0/bits/random.tcc` needs to be patched by turning all `std::max(<constant>, ... )` calls into `std::max((double)<constant>, ...)`

### Team

  * [Joe Dai](https://github.com/jdtech3/)
  * [Swarnava Ghosh](https://github.com/swarnavaghosh04/)
