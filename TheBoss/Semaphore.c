//#include "cmsis_os2.h"                          // CMSIS RTOS header file
// 
///*----------------------------------------------------------------------------
// *      Semaphore creation & usage
// *---------------------------------------------------------------------------*/
// 
//osSemaphoreId_t sid_Semaphore;                  // semaphore id
// 
//osThreadId_t tid_Thread_Semaphore;              // thread id
// 
//void Thread_Semaphore (void *argument);         // thread function
// 
//int Init_Semaphore (void) {
// 
//  sid_Semaphore = osSemaphoreNew(2U, 2U, NULL);
//  if (sid_Semaphore == NULL) {
//    ; // Semaphore object not created, handle failure
//  }
// 
//  tid_Thread_Semaphore = osThreadNew(Thread_Semaphore, NULL, NULL);
//  if (tid_Thread_Semaphore == NULL) {
//    return(-1);
//  }
// 
//  return(0);
//}
// 
//void Thread_Semaphore (void *argument) {
//  int32_t val;
// 
//  while (1) {
//    ; // Insert thread code here...
// 
//    val = osSemaphoreAcquire(sid_Semaphore, 10U);       // wait 10 mSec
//    switch (val) {
//      case osOK:
//        ; // Use protected code here...
//        osSemaphoreRelease(sid_Semaphore);              // return a token back to a semaphore
//        break;
//      case osErrorResource:
//        break;
//      case osErrorParameter:
//        break;
//      default:
//        break;
//    }
// 
//    osThreadYield();                                    // suspend thread
//  }
//}

//izcode

/*

#include "RTE_Components.h"
#include "MKL25Z4.h"
#include "cmsis_os2.h"
#include "driver.h"

// event flags
#define FLAG_CONN ((uint32_t) 0x00000001)
#define FLAG_END  ((uint32_t) 0x00000010)
#define FLAG_MOVE ((uint32_t) 0x00000100)

void tBrain(void *argument);
void tMotor(void *argument);
void tLED(void *argument);
void tEventLED(void *argument);
void tAudio(void *argument);
void tEventAudio(void *argument);

osThreadId_t event_audio_thread, event_led_thread;

osEventFlagsId_t flags;
osMutexId_t green_led_mutex;
osMutexId_t red_led_mutex;
osMutexId_t audio_mutex;

const osThreadAttr_t thread_highprio = {
	.priority = osPriorityHigh
};

const osThreadAttr_t thread_midprio = {
	.priority = osPriorityAboveNormal
};

const osMutexAttr_t mutex_inherit = {
	.attr_bits = osMutexPrioInherit
};

int main(void) {
	SystemCoreClockUpdate();
	InitFrontRearLED();
	InitMotor();
	InitAudio();
	
	osKernelInitialize();
	flags = osEventFlagsNew(NULL);
	green_led_mutex = osMutexNew(&mutex_inherit);
	red_led_mutex = osMutexNew(&mutex_inherit);
	audio_mutex = osMutexNew(&mutex_inherit);
	
	osThreadNew(tBrain, NULL, NULL);
	event_audio_thread = osThreadNew(tEventAudio, NULL, &thread_midprio); 
	event_led_thread = osThreadNew(tEventLED, NULL, &thread_midprio); 
	osThreadNew(tAudio, NULL, NULL);
	osThreadNew(tMotor, NULL, NULL);
	
	osKernelStart();
	for (;;);
}

void tEventLED(void *argument) {
	for (;;) {
		uint32_t events = osThreadFlagsWait(FLAG_CONN, NULL, osWaitForever);
		if (events & FLAG_CONN) { // flash green leds
			osMutexAcquire(green_led_mutex, osWaitForever);	
			
			//...
			osDelay(250);
			//...
			osDelay(250);
			//...
			osDelay(250);
			//...
			osDelay(250);
			
			osMutexRelease(green_led_mutex);
		}
	}
}

void tEventAudio(void *argument) {
	for (;;) {
		uint32_t events = osThreadFlagsWait(FLAG_CONN | FLAG_END, NULL, osWaitForever);
		
		if (events & FLAG_CONN) {
			osMutexAcquire(audio_mutex, osWaitForever);
			//....
			osMutexRelease(audio_mutex);
		}
		
		else if (events & FLAG_END) {
			osMutexAcquire(audio_mutex, osWaitForever);
			//....
			osMutexRelease(audio_mutex);
		}
	}
}

void tAudio(void *argument) {
	uint32_t idx = 0;
	for (;;) {
		osMutexAcquire(audio_mutex, osWaitForever);
		//...
		osMutexRelease(audio_mutex);
		
	}
}


//*/