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
 * REAR RED LEDs
 *---------------------------------------------------------------------------*/
#define PTA1_RLED1  	1										//PTA1_RLED1
#define PTA2_RLED2  	2										//PTA2_RLED2
#define PTD4_RLED3  	4										//PTD4_RLED3
#define PTA12_RLED4 	12										//PTA12_RLED4
#define PTA4_RLED5  	4										//PTA4_RLED5
#define PTA5_RLED6  	5										//PTA5_RLED6
#define PTC10_RLED7  	10										//PTC10_RLED7
#define PTC11_RLED8  	11										//PTC11_RLED8

#define RED_MOVE 500 //when moving
#define RED_STOP 250 //when stationary

/*----------------------------------------------------------------------------
 * FRONT GREEN LEDs
 *---------------------------------------------------------------------------*/
#define PTE5_GLED1  	5							//PTE5_GLED1
#define PTE4_GLED2  	4							//PTE4_GLED2
#define PTE3_GLED3  	3							//PTE3_GLED3
#define PTE2_GLED4  	2							//PTE2_GLED4
#define PTB11_GLED5 	11							//PTB11_GLED5
#define PTB10_GLED6 	10							//PTB10_GLED6
#define PTB9_GLED7  	9							//PTB9_GLED7
#define PTB8_GLED8  	8							//PTB8_GLED8

typedef enum led_number 
{ 
	 led_1 = 0,
	 led_2 = 1,
	 led_3 = 2,
	 led_4 = 3,
	 led_5 = 4,
	 led_6 = 5,
	 led_7 = 6,
	 led_8 = 7
} led_number_t;

/*----------------------------------------------------------------------------
 * Functions
 *---------------------------------------------------------------------------*/

#define OFFLED 0xFE

void InitRGB(void);
void offRGB(void);
void led_controls(enum color_t color);
void make_led_color (enum color_t color);

void initFrontGreenLEDGPIO(void);
void onFrontGreenLED(void);
void offFrontGreenLED(void);
void controlFrontGreenLED(led_number_t number);
void runFrontGreenLED(void);
void twoGreenFlash(void);

void initRearRedLEDGPIO(void);
void offRearRedLED(void);
void flashRearRedLED(int interval);
void stopRearRED(void);
void movingRearRED(void);

#endif // !LED_H
