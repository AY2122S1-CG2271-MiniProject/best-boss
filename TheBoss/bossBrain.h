#include "commonHeads.h"

#ifndef BRAIN_H
#define BRAIN_H

#define AUTOSEMAPHORE					0x02

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
osSemaphoreId_t bossDrive;                  // semaphore id
osSemaphoreId_t bossVoice;                  // semaphore id
osSemaphoreId_t bossAuto;                  // semaphore id

#endif // !BRAIN_H
