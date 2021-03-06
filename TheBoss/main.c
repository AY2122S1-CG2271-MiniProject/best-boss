/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "commonHeads.h"
#include "bossDriver.h"
#include "bossVoice.h"
#include "bossLED.h"
#include "bossBrain.h"
#include "bossUART.h"
#include "bossSensor.h"

#define CONNECTING 0x01
#define CONNECTED 0xFE
#define ENDING 0x99
#define MUTE 0xAA

uint8_t end = 0x00;
uint8_t connect = 0x00;
uint8_t driverless = END_AUTO;

#define MSG_COUNT 1

uint8_t rx_data;
uint8_t muteTrig;
//uint8_t request_t, option_n;

typedef struct {
	uint8_t req_t;
	uint8_t opt_n;
}myDataPack;

osMessageQueueId_t brainQ, driveQ, audioQ, redQ, greenQ, muteQ, sensorQ;

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART2->D;	

		myDataPack myData;
		
		myData.req_t = REQUEST_MASK(rx_data);
		myData.opt_n = OPTION_MASK(rx_data);

		if (rx_data == 0x30) {
			audio_choice = SILENCE;
			stop_music();
			muteTrig = MUTE;
			osMessageQueuePut(audioQ, &myData, NULL, 0);
		} 
		else {
			muteTrig = 0x00;
		}
		if (rx_data == 0x32) {
			audio_choice = ENDING_TIME;
			end = ENDING;
			osMessageQueuePut(audioQ, &myData, NULL, 0);
		}
		else if (rx_data == 0x01) {				//0x01 is Connect
			connect = CONNECTING;
			audio_choice = WIFI_CONNECT;
			osMessageQueuePut(audioQ, &myData, NULL, 0);
		}

		connect = CONNECTED;
		switch (myData.req_t) {
		case DEFAULT_FUNCTIONS:
			//Connect wifi to board (green led flash) & Auto Mode
			//Auto: 0x02 ; StopAuto: 0x03 ; Connect: 0x01
			if (myData.opt_n != CONNECTING) {
				driverless = myData.opt_n;
			} else {
				connect = CONNECTING;
				osMessageQueuePut(audioQ, &myData, NULL, 0);
			}
			break;
		case DRIVEMODES:
			rewrite_driveMode(myData.opt_n);
			osMessageQueuePut(driveQ, &myData, NULL, 0);

			//osMessageQueuePut(brainQ, &myData, NULL, 0);
			break;
		case DIRECTION_OPTIONS:
			rewrite_direction(myData.opt_n);
			osMessageQueuePut(driveQ, &myData, NULL, 0);

			//osMessageQueuePut(brainQ, &myData, NULL, 0);
			break;
		case AUDIO_CONTROL:
			overwriteAudio(myData.opt_n);
			//osMessageQueuePut(brainQ, &myData, NULL, 0);
		default:
			//flashRED;
			stop();
			break;
		}

		osMessageQueuePut(brainQ, &myData, NULL, 0);
	}
}

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/

void bBrain (void *argument) {
	//uint8_t rx_ptr = 0x00;
	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;
  // ...
  for (;;) {
	  osMessageQueueGet(brainQ, &myData, NULL, osWaitForever);
		osMessageQueuePut(audioQ, &myData, NULL, 0);
		osMessageQueuePut(redQ, &myData, NULL, 0);
		osMessageQueuePut(greenQ, &myData, NULL, 0);
		osMessageQueuePut(muteQ, &myData, NULL, 0);
		
		if (driverless == USER_AUTO) {
			osMessageQueuePut(sensorQ, &myData, NULL, 0);
		}
	}
}

volatile float sensorDistance = 0.0;

void bDrive (void *arg) {
	//uint8_t rx_data = INIT;

	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;

	for (;;) {
		osMessageQueueGet(driveQ, &myData, NULL, osWaitForever);
		executeDrive();
		osMessageQueuePut(driveQ, &myData, NULL, 0);
	}
}

void bAudio (void *arg) {

	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;
	audio_choice = SILENCE;

	for(;;) {
		osMessageQueueGet(audioQ, &myData, NULL, 0);
		if (audio_choice == SILENCE) {
			stop_music();
			osMessageQueuePut(brainQ, &myData, NULL, 0); //check!
		}
		else if (audio_choice == ENDING_TIME) {
			play_end_song();
			audio_choice = SILENCE;
			osMessageQueuePut(audioQ, &myData, NULL, 0);
		}
		else if (connect == CONNECTING) {
			audio_choice = WIFI_CONNECT;
			play_wifi_song();
			rx_data = 0x30;
			audio_choice = SILENCE;
			osMessageQueuePut(audioQ, &myData, NULL, 0);
		} 
		else {
			play_moving_song();
			osMessageQueuePut(audioQ, &myData, NULL, 0);
		}
	}
}

void bMute(void* arg) {
	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;

	for (;;) {
		osMessageQueueGet(muteQ, &myData, NULL, osWaitForever);
		if (muteTrig == MUTE) {
			stop_music();
			osMessageQueuePut(muteQ, &myData, NULL, 0);
		}
		else {
			osMessageQueuePut(muteQ, &myData, NULL, 0);
		}
	}
}

void bGreenFront(void *arg) {

	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;

	for(;;) {
		osMessageQueueGet(greenQ, &myData, NULL, 0);
		
		if (connect == CONNECTING) {
			connect = 0x00;
			twoGreenFlash();
			osMessageQueuePut(greenQ, &myData, NULL, 0);
		} else if (connect == 0x00) {
			offFrontGreenLED();
			osMessageQueuePut(greenQ, &myData, NULL, 0);
		}
		else if (isDriving() == 1) {
			runFrontGreenLED();
			osMessageQueuePut(greenQ, &myData, NULL, 0);
		}
		else if (isDriving() == 0) {
			onFrontGreenLED();
			osMessageQueuePut(greenQ, &myData, NULL, 0);
		}
	}
}

void bRedFront(void *arg) {

	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;

	for(;;) {
		osMessageQueueGet(redQ, &myData, NULL, 0);
		if (isDriving() == 1) {
			movingRearRED();
			osMessageQueuePut(redQ, &myData, NULL, 0);
		}
		else if (isDriving() == 0) {
			stopRearRED();
			osMessageQueuePut(redQ, &myData, NULL, 0);
		}
	}
}

void bSensor(void* arg) {
	myDataPack myData;

	myData.req_t = 0xFF;
	myData.opt_n = 0xFF;

	for (;;) {
		osMessageQueueGet(sensorQ, &myData, NULL, osWaitForever);
		pulse();
		osDelay(200);
		executeDrive();
		if (driverless == USER_AUTO) {
			pulse();
			osDelay(20);
			
			//How Far from obstacle
			if (distance < 40) {
				forceDrive(STAYSTILL);
				executeDrive();
				osDelay(1000);
				driverless_mode();
				driverless = MID_AUTO;
			} 
			else if (distance > 40) {
				forceDrive(FORWARD_STRAIGHT);
			}		
		}
		else if (driverless == MID_AUTO) {
			pulse();
			osDelay(20);
			if (distance < 50) {
				forceDrive(STAYSTILL);
				osDelay(10);
				executeDrive();
				driverless = END_AUTO;
			}
			else {
				forceDrive(FORWARD_STRAIGHT);
			}
		}
		else if (driverless == END_AUTO) {
			osMessageQueuePut(brainQ, &myData, NULL, 0);
		}
		osMessageQueuePut(sensorQ, &myData, NULL, 0);
	}
}

int main (void) {
 
  // System Initialization

	SystemCoreClockUpdate();
	initTimer();
	initUltrasonic();

	initUART2();
	InitRGB();
	initFrontGreenLEDGPIO();
	initRearRedLEDGPIO();
	InitMotor(); //+Sensor within
	InitAudio();
	
	stop();
	offRGB();
	offRearRedLED();
	offFrontGreenLED();
	audio_choice = WIFI_CONNECT;
	
	osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(bBrain, NULL, NULL);    // Create application brain thread
	osThreadNew(bDrive, NULL, NULL);		// Create application drive thread
	osThreadNew(bAudio, NULL, NULL);    // Create application audio thread
	osThreadNew(bRedFront, NULL, NULL);		// Create application redLEDs thread
	osThreadNew(bGreenFront, NULL, NULL);    // Create application greenLEDs thread
	osThreadNew(bMute, NULL, NULL);		// Create application to mute speaker thread
	osThreadNew(bSensor, NULL, NULL);		// Create application Sensor thread

	brainQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);
	driveQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);
	audioQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);
	redQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);
	greenQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);
	muteQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);
	sensorQ = osMessageQueueNew(MSG_COUNT, sizeof(myDataPack), NULL);

	osKernelStart();                      // Start thread execution
  for (;;) {}
}
