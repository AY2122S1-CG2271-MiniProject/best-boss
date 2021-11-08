#include "commonHeads.h"

#ifndef BOSSUART_H
#define BOSSUART_H

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

#endif // !BOSSUART_H
