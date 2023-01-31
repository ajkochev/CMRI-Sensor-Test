/*
* CMRI Sensor Test Sketch v1.2-2023
* By Anthony Kochevar
*
* This is a test sketch to test sensors on various ports of an Arduino Mega 
* with a Sensor sheild to see if they are working correctly.
*
* Change anaPin and digiPin as needed to test a particular sensor on that pin.
* Also, numSamples and threshold can also be adjusted as needed to dial in 
* analog sensor readings.
* The sketch changes the status of the CMRI hardware address for JMRI testing and 
* also output the info into the serial monitor.
* The sketch is designed around and Arduino Mega with a Sensor Sheild.
* It starts at port/pin 3 and takes into account the JMRI offset.
* If you have CMRI_ADDR as 2 in JMRI the Hardware address for the sensor 
* would be 2003 as the first address.  for a sensor on port 30 it would be
* 2030.  For a sensor on analog pin A0 (54) it would be 2054.
*
* You will also need the following libraries installed as well:
* https://github.com/madleech/ArduinoCMRI
* https://github.com/madleech/Auto485
*/


#include <CMRI.h>
#include <Auto485.h>


#define CMRI_ADDR 2 //CMRI node address in JMRI.  Change if needed.
#define DE_PIN 2  // Arduino pin used internally for CMRI communication.  Do not change.

Auto485 bus(DE_PIN);
CMRI cmri(CMRI_ADDR, 70, 0, bus);  // Initalize CMRI with up to 70 inputs and 0 outputs.

// Analog pin and variables used
const int anaPin = A0;  // Analog pin used for analog sensor. A0 to A15 (54 to 69) on a Mega.
int anaValue = 0;  // Storage of values taken per numSamples.  Will be divided by numSamples to get average.
const int numSamples = 10;  // Number of readings to get from analog sensors.
const int threshold = 10; // Minimum thresold for tripping the sensor. Adjust value per sensor.

// Digital pin and variables used.
const int digiPin = 3;  //  Pin digital sensor is on, first pin that can be used is 3 to 53 on Mega.


void setup() {

  Serial.begin(19200);
  pinMode(anaPin, INPUT);
  pinMode(digiPin, INPUT);
}

void loop() {

  cmri.process();
  
  bool pinReading = digitalRead(digiPin);
  cmri.set_bit(digiPin - 1, !pinReading); // -1 to account for JMRI offset.  Port 3 on board will be #003 in JMRI.
  Serial.println(F("Digital Sensor"));
  Serial.print(F("Port: ")); Serial.print(digiPin); Serial.print(F("  JMRI: #00")); Serial.println(digiPin);
  Serial.print(F("Value: ")); Serial.println(pinReading);
    if (pinReading == 0) Serial.println(F("Active"));
    else Serial.println(F("In-Active"));
  Serial.println();
  delay(3000);  // Uncomment for debugging.

  anaValue = 0;  //  Reset Analog reading to 0;
  for (int x = 1; x <= numSamples; x++) {  // Take number of numSamples to get an average reading.
	  anaValue += analogRead(anaPin);
    delay(1);  // Pause slightly for debounce.
  }
    anaValue /= numSamples;  // Divide to get the average from numSamples taken. 
    Serial.println(F("Analog Sensor"));
    Serial.print(F("Port: ")); Serial.print(anaPin); Serial.print(F("  JMRI: #0")); Serial.println(anaPin);
    Serial.print(F("Value: ")); Serial.println(anaValue);
    if (anaValue > threshold) {
      cmri.set_bit(anaPin - 1, 1);  // -1 to account for JMRI offset.  Port A0(54) on board will be #054 in JMRI.
      Serial.println(F("Active"));
    }
    else {
      cmri.set_bit(anaPin - 1, 0); // -1 to account for JMRI offset.  Hardware address in JMRI will be #00(anaPin).
      Serial.println(F("In-Active"));
    }
    Serial.println();
    delay(3000);  // Uncomment for debugging.
}
