# Turntable Rotator

## tl;dr

- In theory amazing azimuth control step resolution of 0.02 degrees
- All parts can be 3D printed
- Uses off the shelf NEMA 17 stepper motors and drivers
- Multi-use architecture:tm:
- Universal controller board that can be used for other projects as well

In a nutshell, this project is a simple antenna rotator (could be used for photogrammetry?) that uses a belt driven turntable to rotate the azimuth of the antenna.
Elevation has not been implemented yet, but thanks to modular design you can mount an antenna on the turntable and use it as a simple rotator for azimuth only, attach fixed elevation antennas, or just slap an elevation mechanism on top of the turntable and use it as a full azimuth/elevation rotator.

### Electronics

The controller board is powered by an ESP32-S3 module, can be powered with any 9 - 40 VDC power source and supports two stepper motors and three limit switches.
You can also use servo for elevation instead, there's a pinout for that, need to supply your own power for the servo though.
The board has a USB-C port for programming and debugging.

### Software

SOFTWARE IS NOT YET IMPLEMENTED!!!
~~Commands can be sent over WiFi or UART, and the board can be configured as a WiFi access point or connect to an existing WiFi network.
The board also has a built in web server that can be used to for control, calibration and configuration, but that is also not implemented yet.~~

### Mechanical

First of all, I'm not nowhere competent in mechanical design, this is my first attempt at designing something like this, so if you have any suggestions or improvements please let me know.

The turntable is belt driven, with a 20 tooth pulley on the motor and a 120 tooth pulley on the turntable, giving a 6:1 gear ratio, which means that with a 400 step/rev (0.9 degree/step) stepper motor, we get a theoretical step resolution of 0.02 degrees, which is pretty good for a rotator of this size.
The turntable is suppported by a chungus 6910 bearing, which also allows for cable passage through the center of the turntable.

The turntable is designed to be 3D printed, and all parts can be printed on a standard 3D printer. Due to my lack of design skills, you'd want pretty accurate printer, as the tolerances can get pretty tight, especially for the bearing and screw holes.

### Future Work

- [ ] Redesign the turntable, it was the first part I designed and it shows, the Fusion timeline is a mess and a single change just breaks everything.
- [ ] Design an elevation mechanism, maybe a simple belt driven one, or maybe a servo driven one, not sure yet.
- [ ] Implement the software, need working PCB first.
- [ ] Design pole mount, maybe a simple clamp on design, or maybe a tripod mount, not sure yet.
