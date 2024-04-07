# FlightGPA2
Implementing a flight simulator (term used loosely...) in FPGA on the DE1-SoC: this time with help of Nios II! 

This is a continuation/rewrite of the first version of the project: [FlightGPA](https://github.com/jdtech3/FlightGPA),
where we tried to do the project in Verilog only.

### File organization

  * `fpga/`: Quartus project for running on FPGA: edited version of "DE1-SoC Computer System"
  * `software/`: C/C++ code for running on Nios II core
  * `bsp/`: board support package for Nios II core
  * `host/`: Python code and data on the host computer, i.e. to provide interface to USB joystick

### Patches required

  * `glm` needs to be patched using `glm_PR1264_fix_uninitialized_in_constexpr_warning.patch` taken from [unmerged PR](https://github.com/g-truc/glm/pull/1264)
  * `random.tcc` from `nios2-elf` includes needs to be patched by turning all `std::max(<constant>, ... )` calls into `std::max((double)<constant>, ...)`

### Team

  * [Joe Dai](https://github.com/jdtech3/)
  * [Swarnava Ghosh](https://github.com/swarnavaghosh04/)
