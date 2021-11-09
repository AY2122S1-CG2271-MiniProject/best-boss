#include "commonHeads.h"

#ifndef SENSOR_H
#define SENSOR_H


// SENSOR
#define TRIG_PIN 5 // PortD Pin 5
#define ECHO_PIN 6 // PortD Pin 6

#define PULSE_DELAY 1 // 10us
#define SPEED_OF_SOUND 0.0343 // 343 METRES PER SEC

/*----------------------------------------------------------------------------
 * Functions
 *---------------------------------------------------------------------------*/

float checkDistance(void);
void startCount(void);
void runPulse(void);
void recalibrate(void);

#endif