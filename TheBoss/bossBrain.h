#include "commonHeads.h"
#include "bossUART.h"

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
osSemaphoreId_t tBrain;                  // semaphore id
osSemaphoreId_t bossDrive;                  // semaphore id
osSemaphoreId_t bossLED;                  // semaphore id
osSemaphoreId_t bossVoice;                  // semaphore id
