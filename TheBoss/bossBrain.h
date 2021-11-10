#include "commonHeads.h"

#ifndef BRAIN_H
#define BRAIN_H

#define AUTOSEMAPHORE					0x02

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
osSemaphoreId_t bossDrive;                  // semaphore id
osSemaphoreId_t bossVoice;                  // semaphore id
osSemaphoreId_t bossAuto;  	                // semaphore id

osSemaphoreId_t bossAudio;									//semaphore id
osSemaphoreId_t bossConnect; 

osSemaphoreId_t bossLED;									//semaphore id

osSemaphoreId_t bossAudio2;									//semaphore id
osSemaphoreId_t bossAudio3;									//semaphore id

#endif // !BRAIN_H
