CMRI Sensor Test Sketch v1.2-2023
By Anthony Kochevar

This is a test sketch to test sensors on various ports of an Arduino Mega 
with a Sensor sheild to see if they are working correctly.

Change anaPin and digiPin as needed to test a particular sensor on that pin.
Also, numSamples and threshold can also be adjusted as needed to dial in 
analog sensor readings.

The sketch changes the status of the CMRI hardware address for JMRI testing and 
also outputs the info into the serial monitor.

The sketch is designed around and Arduino Mega with a Sensor Sheild.
It starts at port/pin 3 and takes into account the JMRI offset.
If you have CMRI_ADDR as 2 in JMRI the Hardware address for the sensor 
would be 2003 as the first address.  for a sensor on port 30 it would be
2030.  For a sensor on analog pin A0 (54) it would be 2054.

You will also need the following libraries installed as well:

https://github.com/madleech/ArduinoCMRI

https://github.com/madleech/Auto485
