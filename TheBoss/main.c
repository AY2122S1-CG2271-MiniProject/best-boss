/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "commonHeads.h"
#include "bossDriver.h"
#include "bossVoice.h"
#include "bossLED.h"
#include "bossBrain.h"
#include "bossUART.h"


//osSemaphoreId_t bossBrain;			//semaphore id

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
void bBrain (void *argument) {
  // ...
  for (;;) {
		osSemaphoreAcquire(bossBrain,osWaitForever);
		
		uint8_t request_t, option_n;
		request_t = getType();
		option_n = getOption();
		
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
		executeDrive();
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	
	initUART2();
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
