#include "commonHeads.h"
#include "bossVoice.h"

int notes[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093, 2349, 2637, 2794, 3136, 3520};
int Asharp = 1865;
uint8_t audio_choice = 0x01;

void InitAudio(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	// Configure Mode 3 for speaker PWM
	PORTB->PCR[SPEAKER] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[SPEAKER] |= PORT_PCR_MUX(3);

	// Enable Clock Gating for Timer 1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	uint32_t mod = 48E6 / (128 * 1047);
	TPM1->MOD = mod;
	TPM1_C0V = mod / 3;
	
	//int audio_choice = 0x00;
}

void stop_music(void) {
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) |
		(TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
}

void play_note(uint32_t freq) {
	uint32_t mod = 48E6 / (128 * freq);
	TPM1->MOD = mod;
	TPM1_C0V = mod / 3;
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void overwriteAudio(uint8_t audioChoice) {
	audio_choice = audioChoice;
}

/*
void delays(int rep) {
  int i = rep;
  while (i > 0) {
    osDelay(50);
    i--;
  }
}
//*/

void play_end_song(void) {
	play_note(NOTE_CD);
	osDelay(250);
	play_note(NOTE_CD);
	osDelay(250);
	play_note(NOTE_A);
	osDelay(1250);
	play_note(NOTE_F);
	osDelay(250);
	play_note(NOTE_G);
	osDelay(250);
	play_note(NOTE_F);
	osDelay(250);
	play_note(NOTE_D);
	osDelay(125);
	stop_music();
	osDelay(250);

	play_note(NOTE_D);
	osDelay(250);
	play_note(NOTE_D);
	osDelay(250);
	play_note(Asharp);
	osDelay(1250);
	play_note(NOTE_G);
	osDelay(250);
	play_note(NOTE_A);
	osDelay(250);
	play_note(NOTE_G);
	osDelay(250);
	play_note(NOTE_E);
	osDelay(125);
	stop_music();
	osDelay(250);

	play_note(NOTE_E);
	osDelay(250);
	play_note(NOTE_E);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(1000);
	play_note(NOTE_A);
	osDelay(250);
	play_note(Asharp);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_DU);
	osDelay(125);
	stop_music();
	osDelay(250);

	play_note(NOTE_F);
	osDelay(250);
	play_note(NOTE_G);
	osDelay(250);
	play_note(NOTE_A);
	osDelay(1000);
	play_note(NOTE_G);
	osDelay(1000);
	play_note(NOTE_F);
	osDelay(250);
	play_note(NOTE_F);
	osDelay(125);

	stop_music();
	osDelay(10);
}

void play_moving_song(void) {
	play_note(NOTE_EU);
	osDelay(500);
	play_note(NOTE_B);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_DU);
	osDelay(500);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_B);
	osDelay(250);
	play_note(NOTE_A);
	osDelay(750);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_EU);
	osDelay(500);
	play_note(NOTE_DU);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_B);
	osDelay(500);
	play_note(NOTE_B);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_DU);
	osDelay(500);
	play_note(NOTE_EU);
	osDelay(500);
	play_note(NOTE_CU);
	osDelay(500);
	play_note(NOTE_A);
	osDelay(500);
	play_note(NOTE_A);
	osDelay(500);

	stop_music();
	osDelay(500);
	play_note(NOTE_DU);
	osDelay(750);
	play_note(NOTE_FU);
	osDelay(250);
	play_note(NOTE_AU);
	osDelay(500);
	play_note(NOTE_GU);
	osDelay(250);
	play_note(NOTE_FU);
	osDelay(250);
	play_note(NOTE_EU);
	osDelay(750);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_EU);
	osDelay(500);
	play_note(NOTE_DU);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_B);
	osDelay(500);
	play_note(NOTE_B);
	osDelay(250);
	play_note(NOTE_CU);
	osDelay(250);
	play_note(NOTE_DU);
	osDelay(500);
	play_note(NOTE_EU);
	osDelay(500);
	play_note(NOTE_CU);
	osDelay(500);
	play_note(NOTE_A);
	osDelay(500);
	play_note(NOTE_A);
	osDelay(500);

	stop_music();
	osDelay(200);
}

void play_wifi_song(void) {
	play_note(NOTE_A);
	osDelay(500);
	
	stop_music();
	osDelay(500);
	
	play_note(NOTE_A);
	osDelay(500);
	
	stop_music();
	osDelay(500);
	
	play_note(NOTE_A);
	osDelay(500);
	
	stop_music();
	osDelay(500);
}
