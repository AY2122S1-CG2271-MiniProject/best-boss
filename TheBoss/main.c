/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "commonHeads.h"
#include "bossDriver.h"
#include "bossVoice.h"
#include "bossLED.h"
#include "bossBrain.h"

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

#define USER_AUTO 0x00

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
void bBrain (void *argument) {
  // ...
  for (;;) {
		osSemaphoreAcquire(bossBrain,osWaitForever);
		
		uint8_t request_t, option_n;
		
		request_t = REQUEST_MASK(rx_data);
		option_n = OPTION_MASK(rx_data);
		
		switch (request_t) {
			case DEFAULT_FUNCTIONS:
				//Connect wifi to board (green led flash) & Auto Mode
				break;
			case DRIVEMODES:
				rewrite_driveMode(option_n);
				break;
			case DIRECTION_OPTIONS:
				rewrite_direction(option_n);
				break;
			default:
				//flashRED;
				stop();
				break;
		}
				
		osSemaphoreDelete(bossBrain);
		osSemaphoreRelease(bossDrive);
	}
}

void bDrive (void *arg) {
	for (;;) {
		osSemaphoreAcquire(bossDrive, osWaitForever);
		
		led_controls(RED);
		executeDrive(driveInstructions);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	
	initUART2(BAUD_RATE);
	InitRGB();
	InitFrontRearLED();
	InitMotor();
	InitAudio();
	
	stop();
	offRGB();
	
  // ...
	
	bossBrain = osSemaphoreNew(1,0,NULL);
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(bBrain, NULL, NULL);    // Create application brain thread
	osThreadNew(bDrive, NULL, NULL);		// Create application drive thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
