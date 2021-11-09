#include "commonHeads.h"

//#ifndef BOSSUART_H
//#define BOSSUART_H

#define BAUD_RATE 9600

#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23

#define UART2_INT_PRIO 128

/*----------------------------------------------------------------------------
 * READ data
 *---------------------------------------------------------------------------*/
 
#define REQUEST_SHIFT		0
#define REQUEST_M		0xf0
#define REQUEST_MASK(x) (((uint32_t)(((uint32_t)(x))<<REQUEST_SHIFT))&REQUEST_M)
#define OPTION_M		0x0f
#define OPTION_MASK(x) (((uint32_t)(((uint32_t)(x))))&OPTION_M)

#define DEFAULT_FUNCTIONS 0x00
#define DRIVEMODES 0x10
#define DIRECTION_OPTIONS 0x20
#define AUDIO_CONTROL 0x30

/*----------------------------------------------------------------------------
 * Functions
 *---------------------------------------------------------------------------*/
void initUART2(void);
void UART2_IRQHandler(void);
uint8_t getType(void);
uint8_t getOption(void);

//#endif // !BOSSUART_H

osSemaphoreId_t bossBrain;			//semaphore id

void initUART2() {
	uint32_t divisor, bus_clock;
	//enable clock to UART2 and PORTE
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//connect UART pins for PTE23
	PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);
	
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	//Ensure Tx and Rx are disabled before configuration
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	//Set Baud Rate to desired value 
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2; //bus clock is always half in KL25Z
	divisor  = bus_clock / (BAUD_RATE * 16); //the formula in the notes
	UART2->BDH = UART_BDH_SBR(divisor >> 8); //baud rate register high
	UART2->BDL = UART_BDL_SBR(divisor); //baud rate register low
	
	//no parity, 8-bits
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	NVIC_SetPriority(UART2_IRQn, 128);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	//enable Rx
	UART2->C2 |= (UART_C2_RE_MASK);
	UART2->C2 |= UART_C2_RIE_MASK;
}
