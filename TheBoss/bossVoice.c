#include "commonHeads.h"
#include "bossVoice.h"

int notes[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093, 2349, 2637, 2794, 3136, 3520};
int Asharp = 1865;

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
}

void play_note(uint32_t freq) {
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) |
		(TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	uint32_t mod = 48E6 / (128 * freq);
	TPM1->MOD = mod;
	TPM1_C0V = mod / 2;
	TPM1_CNT = 0;
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void stop_music(void) {
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) |
		(TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
}

void play_end_song(void) {
	play_note(NOTE_CD);
	osDelay(800);
	play_note(NOTE_CD);
	osDelay(800);
	play_note(NOTE_A);
	osDelay(4800);
	play_note(NOTE_F);
	osDelay(800);
	play_note(NOTE_G);
	osDelay(800);
	play_note(NOTE_F);
	osDelay(800);
	play_note(NOTE_D);
	osDelay(4000);
	stop_music();
	osDelay(800);

	play_note(NOTE_D);
	osDelay(800);
	play_note(NOTE_D);
	osDelay(800);
	play_note(Asharp);
	osDelay(4800);
	play_note(NOTE_G);
	osDelay(800);
	play_note(NOTE_A);
	osDelay(800);
	play_note(NOTE_G);
	osDelay(800);
	play_note(NOTE_E);
	osDelay(4000);
	stop_music();
	osDelay(800);

	play_note(NOTE_E);
	osDelay(800);
	play_note(NOTE_E);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(4800);
	play_note(NOTE_A);
	osDelay(800);
	play_note(Asharp);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_DU);
	osDelay(4000);
	stop_music();
	osDelay(800);

	play_note(NOTE_F);
	osDelay(800);
	play_note(NOTE_G);
	osDelay(800);
	play_note(NOTE_A);
	osDelay(3200);
	play_note(NOTE_G);
	osDelay(3200);
	play_note(NOTE_F);
	osDelay(800);
	play_note(NOTE_F);
	osDelay(4000);

	stop_music();
}

void play_moving_song(void) {
	play_note(NOTE_EU);
	osDelay(1600);
	play_note(NOTE_B);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_DU);
	osDelay(1600);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_B);
	osDelay(800);
	play_note(NOTE_A);
	osDelay(2400);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_EU);
	osDelay(1600);
	play_note(NOTE_DU);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_B);
	osDelay(1600);
	play_note(NOTE_B);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_DU);
	osDelay(1600);
	play_note(NOTE_EU);
	osDelay(1600);
	play_note(NOTE_CU);
	osDelay(1600);
	play_note(NOTE_A);
	osDelay(1600);
	play_note(NOTE_A);
	osDelay(1600);

	stop_music();
	osDelay(1600);
	play_note(NOTE_DU);
	osDelay(2400);
	play_note(NOTE_FU);
	osDelay(800);
	play_note(NOTE_AU);
	osDelay(1600);
	play_note(NOTE_GU);
	osDelay(800);
	play_note(NOTE_FU);
	osDelay(800);
	play_note(NOTE_EU);
	osDelay(2400);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_EU);
	osDelay(1600);
	play_note(NOTE_DU);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_B);
	osDelay(1600);
	play_note(NOTE_B);
	osDelay(800);
	play_note(NOTE_CU);
	osDelay(800);
	play_note(NOTE_DU);
	osDelay(1600);
	play_note(NOTE_EU);
	osDelay(1600);
	play_note(NOTE_CU);
	osDelay(1600);
	play_note(NOTE_A);
	osDelay(1600);
	play_note(NOTE_A);
	osDelay(1600);

	stop_music();
}
