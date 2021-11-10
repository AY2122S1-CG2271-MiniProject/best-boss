#include "commonHeads.h"

#ifndef SENSOR_H
#define SENSOR_H


// SENSOR
#define ECHO_PIN 13 // PortA Pin 13
#define TRIG_PIN 6 // PortD Pin 6

#define PULSE_DELAY 1 // 10us
#define SPEED_OF_SOUND 0.0343 // 343 METRES PER SEC

/*----------------------------------------------------------------------------
 * Functions
 *---------------------------------------------------------------------------*/

void initUltrasonic(void);
float checkDistance(void);
void startCount(void);
void runPulse(void);
void recalibrate(void);
int readUltrasonic(void);
void TPM1_IRQHandler(void);
void ultrasonicTrigger(void);
void delay(volatile uint32_t nof);

#endif
