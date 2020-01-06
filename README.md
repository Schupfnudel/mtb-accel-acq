# mtb-accel-acq
Reading acceleration data from 3 MEMS accelerometers and saving them to SPI SD card, used to analyse behavior of MTB forks and shocks

Hardware:

Adafruit Itsybitsy M4 Express MCU

SPI microSD Reader

MPU6050 Accelerometers

The accelerometers are mounted on the handlebar, the fork and the left chainstay respectively. This is so I can differentiate between the forces from the front or back wheel, thus isolating the influence of the fork or the shock settings. The mounts are 3D printed from PLA. This is a relatively stiff material compared to other plastics I can 3d print, actually the best I can do. The breakout boards are fastened in the mounts with CA glue, that is also relatively stiff and should be fine. Screws do not work with my specific breakout boards because there are some passives mounted way too close to the screwholes... maybe try some other boards so you can screw and glue them in to avoid resonant oscillations of the pcb. Just dont use double sided tape or something similarly soft ;)
Ideally the mounts would be fastened to the bike with more screws or welds, but I like my bike without any holes in the frame, so the best I can do again is zipties. Not ideal for the chainstay, probably fine for the other two accelerometers. The holder for the batteries, MCU and SD card reader is not really critical, I quickly whipped up a holder to screw to the bottlecage. I dont inted to keep this setup on my bike for a long time, so I did not bother with waterproofing, everything is easily accessible.

TODO:
- the SD reader permanently blocks SPI bus, CS does nothing. So I had to use the accelerometers with (slower) I2C. The maximum ODR of the MPU6050 is 1000Hz, so speed is not super critical, but I will try and find a better SD card reader.
- the SD card is not fast enough to write data in real time, big SPI flash would be better. For the moment I used onboard RAM of the M4 to buffer data for 5 seconds and then stop reading to save to SD card. There are 2MB SPI flash on the board too, but thats still not enough to buffer a full run and reading from the flash to SD card would take even longer, so I went with shorter bursts of data.
- 1kHz ODR and +-16g range are just guesses, after some tests slower ODR might enable real time data acquisition or smaller ranges might provide higher resolution
- actual data evaluation, probably in matlab when it gets warm enough for damping circuits to work properly
