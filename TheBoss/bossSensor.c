#include "bossSensor.h"
#include "math.h"

#define MASK(x) (1 << (x))

volatile int ready = 0;
volatile uint16_t start = 0;
volatile uint16_t startRecord = 0;
volatile uint16_t endRecord = 0;
volatile uint16_t valueRecord = 0;


volatile uint16_t ends = 0;
volatile uint16_t counters = 0;
volatile uint16_t read = 0;
volatile uint16_t num = 0;

volatile uint16_t maincounter = 0;

void delay(volatile uint32_t nof) {
	while (nof != 0) {
		__asm("NOP");
		nof--;
	}
}

void ultrasonicTrigger(void) {

	//starting trigger
	PTD->PDOR |= MASK(TRIG_PIN);
	delay(0x11); //10us orginal = 0x11

	//Off trigger after 10 seconds
	PTD->PDOR &= ~MASK(TRIG_PIN);
	delay(0x18e70); // 60ms original = 0x18e70
}

int readUltrasonic(void) {
	float value = 0;

	// Enable Clk Source to TPM1 module
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

	ultrasonicTrigger();

	while (!ready) {}; // Measuring when echo comesback
	value = ends - startRecord;
	start = 0;
	value = (value * 2.6666 * 0.01715 * 1.5) - 337;  // determine speed of ultrasonic from frequency and speedy of sound = 34300cm/s /2 =17150;

	// Disable Clk to avoid interrupt triggers
	SIM->SCGC6 &= ~SIM_SCGC6_TPM1_MASK;
	counters = 0;

	return value;
}

void TPM1_IRQHandler(void) // What is the interrupt logic
{
	NVIC_ClearPendingIRQ(TPM1_IRQn); // Clear queue before begin;

	if (TPM1_C1SC & TPM_CnSC_CHF_MASK) {  // If CHF = channel is 1 when an event occur- cleared by writing 1 
		TPM1_C1SC |= TPM_CnSC_CHF_MASK;
		if (!start) {
			// Clear the TPM1 counter register to start with ~0
			TPM1_CNT = 0x00000000000;
			startRecord = TPM1_CNT;
			start = 1;
		}
		else {
			ends = TPM1_C1V + counters * 7500; // TPM1_C1V is counter value
			ready = 1;
		}
	}
	if (TPM1_SC & TPM_SC_TOF_MASK) { // Checking for overflow so that it resets when hit and counter = 1 cycle.
		TPM1_SC |= TPM_SC_TOF_MASK;
		counters++;
	}
}
