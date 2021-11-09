#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "MKL25Z4.h"
#define MASK32(x) ((uint32_t)(1 << ((uint32_t)x))) // Changes all bits to 0 except x
#define MASK(x) (1 << (x))

extern osSemaphoreId_t bossBrain;			//semaphore id
extern osSemaphoreId_t bossDrive;                  // semaphore id
extern osSemaphoreId_t bossLED;                  // semaphore id
extern osSemaphoreId_t bossVoice;                  // semaphore id
extern osSemaphoreId_t bossConnect;                  // semaphore id

extern int auto_modeOn;
extern uint8_t audio_choice;
