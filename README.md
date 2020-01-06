# mtb-accel-acq
Reading acceleration data from 3 MEMS accelerometers and saving them to SPI SD card, used to analyse behavior of MTB forks and shocks

Hardware:

Adafruit Itsybitsy M4 Express MCU
SPI microSD Reader
MPU6050 Accelerometers

TODO:
- the SD reader permanently blocks SPI bus, CS does nothing. So I had to use the accelerometers with (slower) I2C. The maximum ODR of the MPU6050 is 1000Hz, so speed is not super critical, but I will try and find a better SD card reader.
- the SD card is not fast enough to write data in real time, big SPI flash would be better. For the moment I used onboard RAM of the M4 to buffer data for 5 seconds and then stop reading to save to SD card. There are 2MB SPI flash on the board too, but thats still not enough to buffer a full run and reading from the flash to SD card would take even longer, so I went with shorter bursts of data.
- 1kHz ODR and +-16g range are just guesses, after some tests slower ODR might enable real time data acquisition or smaller ranges might provide higher resolution
- actual data evaluation, probably in matlab when it gets warm enough for damping circuits to work properly
