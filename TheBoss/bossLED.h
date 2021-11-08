#include "commonHeads.h"

#ifndef LED_H
#define LED_H

/*----------------------------------------------------------------------------
 * Board LEDs
 *---------------------------------------------------------------------------*/
#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1

enum color_t {NONE, RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, WHITE, OFFRED, OFFGREEN, OFFBLUE};

/*----------------------------------------------------------------------------
 * FRONT & READ LEDs
 *---------------------------------------------------------------------------*/
#define REAR_LED 16 // PortC Pin 16

#define FRONT_LEDS_LEN 10

/*----------------------------------------------------------------------------
 * Functions
 *---------------------------------------------------------------------------*/

void InitRGB(void);
void offRGB(void);
void led_controls(enum color_t color);
void force_led_color (enum color_t color);
void InitFrontRearLED(void);

#endif // !LED_H
