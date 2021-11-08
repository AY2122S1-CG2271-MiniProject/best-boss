#include "commonHeads.h"

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

const int FRONT_LEDS[] = {7, 0, 3, 4, 5, 6, 10, 11, 12, 13}; // PortC Pin 7 upwards to Pin 13
#define FRONT_LEDS_LEN (sizeof(FRONT_LEDS) / sizeof(FRONT_LEDS[0]))

/*----------------------------------------------------------------------------
 * Board LEDs
 *---------------------------------------------------------------------------*/

void InitRGB(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK32(RED_LED) | MASK32(GREEN_LED));
	PTD->PDDR |= MASK32(BLUE_LED);
}

void offRGB() {
	PTB->PDOR |= MASK32(RED_LED) | MASK32(GREEN_LED); // Switch off GREEN and RED LED
	PTD->PDOR |= MASK32(BLUE_LED); // Switch off BLUE LED
}

void led_controls(enum color_t color) {
	switch(color) {
    case NONE:
			break;
    case RED:
      PTB->PDOR &= ~MASK32(RED_LED); // Switch on RED LED
      break;
    case GREEN:
      PTB->PDOR &= ~MASK32(GREEN_LED); // Switch on GREEN LED
      break;
    case BLUE:
      PTD->PDOR &= ~MASK32(BLUE_LED); // Switch on BLUE LED
      break;
    case MAGENTA:
      PTB->PDOR &= ~MASK32(RED_LED); // Switch on RED LED
      PTD->PDOR &= ~MASK32(BLUE_LED); // Switch on BLUE LED
      break;
    case CYAN:
      PTB->PDOR &= ~MASK32(GREEN_LED); // Switch on GREEN LED
      PTD->PDOR &= ~MASK32(BLUE_LED); // Switch on BLUE LED
      break;
    case YELLOW:
      PTB->PDOR &= ~MASK32(RED_LED); // Switch on RED LED
      PTB->PDOR &= ~MASK32(GREEN_LED); // Switch on GREEN LED
      break;
    case WHITE:
			PTB->PDOR &= ~MASK32(RED_LED) & ~MASK32(GREEN_LED); // Switch on RED and GREEN LED
			PTD->PDOR &= ~MASK32(BLUE_LED); // Switch on BLUE LED
      break;
		case OFFRED:
			PTB->PDOR |= MASK32(RED_LED); // Switch off RED LED
			break;
		case OFFGREEN:
			PTB->PDOR |= MASK32(GREEN_LED); // Switch off GREEN
			break;
		case OFFBLUE:
			PTD->PDOR |= MASK32(BLUE_LED); // Switch off BLUE LED
			break;
	}
}

void force_led_color (enum color_t color) {
	offRGB();
	led_controls(color);
}
		
/*----------------------------------------------------------------------------
 * FRONT & READ LEDs
 *---------------------------------------------------------------------------*/
void InitFrontRearLED(void) {
    // Enable Clock Gating for PORTC
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
		// Configure MUX to make pins GPIO
		///*
		for (int i = 0; i < FRONT_LEDS_LEN; i++) {				
		    PORTC->PCR[FRONT_LEDS[i]] &= ~PORT_PCR_MUX_MASK;
        PORTC->PCR[FRONT_LEDS[i]] |= PORT_PCR_MUX(1);
    }//*/
    PORTC->PCR[REAR_LED] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[REAR_LED] |= PORT_PCR_MUX(1);
		
    // Set Data Direction Registers for PortC
    for (int i = 0; i < FRONT_LEDS_LEN; i++) {
        PTC->PDDR |= MASK32(FRONT_LEDS[i]);
    }
    PTC->PDDR |= MASK32(REAR_LED);
}
