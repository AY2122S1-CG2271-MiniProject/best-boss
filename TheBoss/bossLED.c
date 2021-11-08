#include "bossLED.h"

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
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}

void offRGB(void) {
	PTB->PDOR |= MASK(RED_LED) | MASK(GREEN_LED); // Switch off GREEN and RED LED
	PTD->PDOR |= MASK(BLUE_LED); // Switch off BLUE LED
}

void led_controls(enum color_t color) {
	switch(color) {
    case NONE:
			break;
    case RED:
      PTB->PDOR &= ~MASK(RED_LED); // Switch on RED LED
      break;
    case GREEN:
      PTB->PDOR &= ~MASK(GREEN_LED); // Switch on GREEN LED
      break;
    case BLUE:
      PTD->PDOR &= ~MASK(BLUE_LED); // Switch on BLUE LED
      break;
    case MAGENTA:
      PTB->PDOR &= ~MASK(RED_LED); // Switch on RED LED
      PTD->PDOR &= ~MASK(BLUE_LED); // Switch on BLUE LED
      break;
    case CYAN:
      PTB->PDOR &= ~MASK(GREEN_LED); // Switch on GREEN LED
      PTD->PDOR &= ~MASK(BLUE_LED); // Switch on BLUE LED
      break;
    case YELLOW:
      PTB->PDOR &= ~MASK(RED_LED); // Switch on RED LED
      PTB->PDOR &= ~MASK(GREEN_LED); // Switch on GREEN LED
      break;
    case WHITE:
			PTB->PDOR &= ~MASK(RED_LED) & ~MASK(GREEN_LED); // Switch on RED and GREEN LED
			PTD->PDOR &= ~MASK(BLUE_LED); // Switch on BLUE LED
      break;
		case OFFRED:
			PTB->PDOR |= MASK(RED_LED); // Switch off RED LED
			break;
		case OFFGREEN:
			PTB->PDOR |= MASK(GREEN_LED); // Switch off GREEN
			break;
		case OFFBLUE:
			PTD->PDOR |= MASK(BLUE_LED); // Switch off BLUE LED
			break;
	}
}

void make_led_color (enum color_t color) {
	offRGB();
	led_controls(color);
}

/*----------------------------------------------------------------------------
 * REAR LEDs
 *---------------------------------------------------------------------------*/

void initRearRedLEDGPIO(void) {
	// Enable Clock Gating to PORTA, PORTC and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTA_MASK) | (SIM_SCGC5_PORTC_MASK)| (SIM_SCGC5_PORTD_MASK));
	
	// Make pins GPIO
	PORTA->PCR[PTA1_RLED1] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA1_RLED1] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[PTA2_RLED2] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA2_RLED2] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[PTD4_RLED3] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD4_RLED3] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[PTA12_RLED4] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA12_RLED4] |= PORT_PCR_MUX(1);
	
	PORTA->PCR[PTA4_RLED5] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA4_RLED5] |= PORT_PCR_MUX(1);

	PORTA->PCR[PTA5_RLED6] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[PTA5_RLED6] |= PORT_PCR_MUX(1);
	
	PORTC->PCR[PTC8_RLED7] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[PTC8_RLED7] |= PORT_PCR_MUX(1);
	
	PORTC->PCR[PTC9_RLED8] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[PTC9_RLED8] |= PORT_PCR_MUX(1);
	
	// Set pins as output
	PTA->PDDR |= (MASK(PTA1_RLED1) | 
								MASK(PTA2_RLED2) | 
								MASK(PTA4_RLED5) | 
								MASK(PTA5_RLED6) | 
								MASK(PTA12_RLED4));
								
	PTC->PDDR |= (MASK(PTC8_RLED7) | 
								MASK(PTC9_RLED8));
								
	PTD->PDDR |= 	MASK(PTD4_RLED3);
}

// 8 Rear Red LEDs are switched on
void onRearRedLED(void) {
	//set output register
	PTA->PSOR |= (MASK(PTA1_RLED1) | 
								MASK(PTA2_RLED2) | 
								MASK(PTA4_RLED5) | 
								MASK(PTA5_RLED6) | 
								MASK(PTA12_RLED4));
	
	PTC->PSOR |= (MASK(PTC8_RLED7) | 
								MASK(PTC9_RLED8));
	
	PTD->PSOR |= 	MASK(PTD4_RLED3);
}

// 8 Rear Red LEDs are switched off
void offRearRedLED(void) {
	//clear output register
	PTA->PCOR |= (MASK(PTA1_RLED1) | 
								MASK(PTA2_RLED2) | 
								MASK(PTA4_RLED5) | 
								MASK(PTA5_RLED6) | 
								MASK(PTA12_RLED4));
	
	PTC->PCOR |= (MASK(PTC8_RLED7) | 
								MASK(PTC9_RLED8));
	
	PTD->PCOR |= 	MASK(PTD4_RLED3);
}

// 8 Rear Red LEDs flash at a rate depending on if the robot is moving (500ms) or stationary (250ms)
void flashRearRedLED(int interval) {
	onRearRedLED();
	osDelay(interval);
	offRearRedLED();
  osDelay(interval);
}

void stopRearRED(void) {
	flashRearRedLED(RED_STOP);
}

void movingRearRED(void) {
	flashRearRedLED(RED_MOVE);
}	

/*----------------------------------------------------------------------------
 * FRONT LEDs
 *---------------------------------------------------------------------------*/

const int FRONT_LEDS[] = {7, 0, 3, 4, 5, 6, 10, 11, 12, 13}; // PortC Pin 7 upwards to Pin 13

// Initialises GPIO for 8 Front Green LEDs
void initFrontGreenLEDGPIO(void) {
	// Enable Clock Gating to PORTB and PORTE
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTE_MASK));
	
	// Make pins GPIO
	PORTE->PCR[PTE5_GLED1] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE5_GLED1] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[PTE4_GLED2] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE4_GLED2] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[PTE3_GLED3] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE3_GLED3] |= PORT_PCR_MUX(1);
	
	PORTE->PCR[PTE2_GLED4] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[PTE2_GLED4] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PTB11_GLED5] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB11_GLED5] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PTB10_GLED6] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB10_GLED6] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PTB9_GLED7] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB9_GLED7] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[PTB8_GLED8] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB8_GLED8] |= PORT_PCR_MUX(1);
	
	// Set pins as output	
	PTB->PDDR |= (MASK(PTB11_GLED5) | 
								MASK(PTB10_GLED6) | 
								MASK(PTB9_GLED7) | 
								MASK(PTB8_GLED8));	
								
	PTE->PDDR |= (MASK(PTE5_GLED1) | 
								MASK(PTE4_GLED2) | 
								MASK(PTE3_GLED3) | 
								MASK(PTE2_GLED4));
}

// 8 Front Green LEDs are switched on; when robot is stationary
void onFrontGreenLED(void) {
	//set output register
	PTB->PSOR |= (MASK(PTB11_GLED5) | 
								MASK(PTB10_GLED6) | 
								MASK(PTB9_GLED7) | 
								MASK(PTB8_GLED8));	
	
	PTE->PSOR |= (MASK(PTE5_GLED1) | 
								MASK(PTE4_GLED2) | 
								MASK(PTE3_GLED3) | 
								MASK(PTE2_GLED4));
}

// 8 Front Green LEDs are switched off
void offFrontGreenLED(void) {
	//clear output register
	PTB->PCOR |= (MASK(PTB11_GLED5) | 
								MASK(PTB10_GLED6) | 
								MASK(PTB9_GLED7) | 
								MASK(PTB8_GLED8));	
	
	PTE->PCOR |= (MASK(PTE5_GLED1) | 
								MASK(PTE4_GLED2) | 
								MASK(PTE3_GLED3) | 
								MASK(PTE2_GLED4));
}

// Logic for 8 Front Green LEDs' Running Mode
void controlFrontGreenLED(led_number_t number) {
	offFrontGreenLED();
	
	switch(number) {
	case led_1:
		PTE->PSOR = MASK(PTE5_GLED1);
		break;
	
	case led_2:
		PTE->PSOR = MASK(PTE4_GLED2);
		break;

	case led_3:
		PTE->PSOR = MASK(PTE3_GLED3);
		break;

	case led_4:
		PTE->PSOR = MASK(PTE2_GLED4);
		break;

	case led_5:
		PTB->PSOR = MASK(PTB11_GLED5);
		break;

	case led_6:
		PTB->PSOR = MASK(PTB10_GLED6);
		break;		

	case led_7:
		PTB->PSOR = MASK(PTB9_GLED7);
		break;
	
	case led_8:
		PTB->PSOR = MASK(PTB8_GLED8);
		break;

	default:
		offFrontGreenLED();
	}
}

uint8_t counter = 0;
int ascending;

char led_mapping[8][2] = 
{
	{0, led_1},
	{1, led_2}, 
	{2, led_3},
	{3, led_4}, 
	{4, led_5},
	{5, led_6}, 
	{6, led_7},
	{7, led_8}
};
// 8 Front Green LEDs is in Running Mode (1 LED at a time); when robot is moving
void runFrontGreenLED(void) {
	ascending = (counter == 0x07) ? 0 : ((counter == 0x00) ? 1 : ascending);
	counter += (ascending == 1) ? 1 : -1;
	controlFrontGreenLED(led_mapping[counter][1]);
	osDelay(100);
}

//8 Front Green LED flashes after establishing wifi
void twoGreenFlash(void) {
	for (int i = 0; i < 2 ; i++) {
		onFrontGreenLED();
		osDelay(150);
		offFrontGreenLED();
		osDelay(150);
		statusUpdate = 0;
	}
}
