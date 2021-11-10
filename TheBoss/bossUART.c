#include "bossUART.h"

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
	bus_clock = (DEFAULT_SYSTEM_CLOCK) / 2; //bus clock is always half in KL25Z
	divisor = bus_clock / (BAUD_RATE * 16); //the formula in the notes
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
