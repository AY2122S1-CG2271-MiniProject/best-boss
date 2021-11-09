/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "commonHeads.h"
#include "bossDriver.h"
#include "bossVoice.h"
#include "bossLED.h"
#include "bossBrain.h"
#include "bossUART.h"
//#include "bossSensor.h"

//osSemaphoreId_t bossBrain;			//semaphore id

uint8_t rx_data;

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;	
		osSemaphoreRelease(bossBrain);
	}
}

/*
uint8_t getType(void) {
	return REQUEST_MASK(rx_data);
}

uint8_t getOption(void) {
	return OPTION_MASK(rx_data);
}
*/

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
uint8_t savedStatus = 0x00;

int statusGet(void) {
	return savedStatus;
}

//<<<<<<< HEAD
//=======
int statusUpdate = 1; //original is 0
//>>>>>>> testingGrounds
void handleConnection(uint8_t option) {
	savedStatus = rx_data;
}

void bBrain (void *argument) {
  // ...
  for (;;) {
		osSemaphoreAcquire(bossBrain,osWaitForever);
		
		uint8_t request_t, option_n;
		request_t = REQUEST_MASK(rx_data);
		option_n = OPTION_MASK(rx_data);
		
		switch (request_t) {
			case DEFAULT_FUNCTIONS:
				handleAutoSwitch(option_n);
				if (option_n == AUTOSEMAPHORE) {
					osSemaphoreRelease(bossAuto);
				}
				else if (option_n == WIFI_CONNECT) {
				handleConnection(option_n);
				}
				//Connect wifi to board (green led flash) & Auto Mode
				//Auto: 0x02 ; StopAuto: 0x03 ; Connect: 0x01
				break;
			case DRIVEMODES:
				rewrite_driveMode(option_n);
				break;
			case DIRECTION_OPTIONS:
				rewrite_direction(option_n);
				break;
			case AUDIO_CONTROL:
				overwriteAudio(option_n);
				switch(audio_choice) {
					case SILENCE:
						stop_music();
						break;
					case ENDING_TIME:
						play_end_song();
						audio_choice = SILENCE;
						break;
					case WIFI_CONNECT:
						play_wifi_song();
						audio_choice = SILENCE;
						break;
					default:
						stop_music();
						break;
				}
			default:
				//flashRED;
				stop();
				break;
		}
		osSemaphoreDelete(bossBrain);
		
		//If auto mode is off dont release user drive
		if (auto_modeOn == 0) {
			osSemaphoreRelease(bossDrive);
		}
	}
}

void bDrive (void *arg) {
	for (;;) {
		osSemaphoreAcquire(bossDrive, osWaitForever);
		executeDrive();
		osSemaphoreRelease(bossLED);
	}
}

volatile float sensorDistance = 0.0;

void bAuto (void *arg) {
	for (;;) {
		osSemaphoreAcquire(bossAuto, osWaitForever);
		//sensorDistance = checkDistance();
		//driverless_mode(AUTO, sensorDistance);
	}
}

void bAudio (void *arg) {
	for(;;) {
		osSemaphoreAcquire(bossAudio, osWaitForever);
		play_moving_song();
		osSemaphoreDelete(bossAudio);
	}
}

void bIntAudio (void *arg) {
	osSemaphoreAcquire(bossAudio, osWaitForever);
	switch(audio_choice) {
		case SILENCE:
			stop_music();
			break;
		case ENDING_TIME:
			play_end_song();
			audio_choice = SILENCE;
			break;
		case WIFI_CONNECT:
			play_wifi_song();
			audio_choice = SILENCE;
			break;
	}
}

void bConnect(void*arg) {
	for (;;) {
		osSemaphoreAcquire(bossConnect, osWaitForever);
		twoGreenFlash();
		osSemaphoreRelease(bossLED);
	}
}

void bGreenFront(void *arg) {
	for(;;) {
		osSemaphoreAcquire(bossLED, osWaitForever);
		if (isDriving() == 1) {
			runFrontGreenLED();
		}
		else if (isDriving() == 0) {
			onFrontGreenLED();
		}
	}
}

void bRedFront(void *arg) {
	for(;;) {
		if (isDriving() == 1) {
			movingRearRED();
		}
		else if (isDriving() == 0) {
			stopRearRED();
		}
	}
}
 
int main (void) {
 
  // System Initialization
	SystemCoreClockUpdate();
	
	initUART2();
	InitRGB();
	initFrontGreenLEDGPIO();
	initRearRedLEDGPIO();
	InitMotor();
	InitAudio();
	
	stop();
	offRGB();
	offRearRedLED();
	offFrontGreenLED();
	
  // ...
	
	bossBrain = osSemaphoreNew(1,0,NULL);
	bossAuto = osSemaphoreNew(1,0,NULL);
	bossConnect = osSemaphoreNew(1,0,NULL);
	bossLED = osSemaphoreNew(1,0,NULL);
 
	osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(bBrain, NULL, NULL);    // Create application brain thread
	osThreadNew(bDrive, NULL, NULL);		// Create application drive thread
	osThreadNew(bAudio, NULL, NULL);    // Create application brain thread
	osThreadNew(bRedFront, NULL, NULL);		// Create application drive thread
	osThreadNew(bGreenFront, NULL, NULL);    // Create application brain thread
	osThreadNew(bAuto, NULL, NULL);		// Create application drive thread
	osKernelStart();                      // Start thread execution
  for (;;) {}
}
