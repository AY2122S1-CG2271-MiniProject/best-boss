#include "bossSensor.h"
#include "math.h"

volatile float distance=0.0; 
volatile float duration=0.0;
unsigned long count = 0;
volatile int flag = 0;

void runPulse(){
	
	TPM1->MOD = 9375; // 25hz
	TPM1_C0V = 9375; 
}

float checkDistance(){
	
		if(flag == 0){
			PTD -> PCOR |= MASK(TRIG_PIN);
			PTD -> PCOR |= MASK(ECHO_PIN);
			count = 0;
			distance = 0;
			runPulse();
		
			while(!(PTD->PDIR == 0x0000004C || PTD->PDIR == 0x000000CC || PTD->PDIR == 0X0000004D || PTD->PDIR == 0X000000CD)){

			}; //Wait for a HIGH on ECHO pin. Once high then go to next line
		
			while(PTD->PDIR == 0x0000004C || PTD->PDIR == 0x000000CC || PTD->PDIR == 0X0000004D || PTD->PDIR == 0X000000CD){//Wait until ECHO pin is LOW. If low, then exits loop
				count++;
				if(count == 0xff){
					duration++;
					count=0;
				}
			}; 
			
			flag = 1;
			return distance = (SPEED_OF_SOUND * count)/2; //Find the distance, in cm
			//flag = 1;
			
			}else{
				return distance;
			}
}

void recalibrate(){
	flag = 0;
}

void startCount(){
		count++;
		if(count == 20971520){ // 20971520 (20Mhz)
		duration++; // hopefully 1 sec
		}
}

