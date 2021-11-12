#include "commonHeads.h"
#include "bossVoice.h"

int notes[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093, 2349, 2637, 2794, 3136, 3520};
int Asharp = 1865;
uint8_t audio_choice = 0x01;

void InitAudio(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	// Configure Mode 3 for speaker PWM
	PORTA->PCR[SPEAKER] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[SPEAKER] |= PORT_PCR_MUX(3);

	// Enable Clock Gating for Timer 1
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	uint32_t mod = 48E6 / (128 * 1047);
	TPM2->MOD = mod;
	TPM2_C0V = mod / 3;
}

void stop_music(void) {
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) |
		(TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
}

void play_note(uint32_t freq) {
	uint32_t mod = 48E6 / (128 * freq);
	TPM2->MOD = mod;
	TPM2_C0V = mod / 3;
	
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void overwriteAudio(uint8_t audioChoice) {
	audio_choice = audioChoice;
}

void play_end_song(void) {
	play_note(NOTE_CD);
	osDelay(157);
	play_note(NOTE_CD);
	osDelay(157);
	play_note(NOTE_A);
	osDelay(780);
	play_note(NOTE_F);
	osDelay(157);
	play_note(NOTE_G);
	osDelay(157);
	play_note(NOTE_F);
	osDelay(157);
	play_note(NOTE_D);
	osDelay(125);
	stop_music();
	osDelay(157);

	play_note(NOTE_D);
	osDelay(157);
	play_note(NOTE_D);
	osDelay(157);
	play_note(Asharp);
	osDelay(780);
	play_note(NOTE_G);
	osDelay(157);
	play_note(NOTE_A);
	osDelay(157);
	play_note(NOTE_G);
	osDelay(157);
	play_note(NOTE_E);
	osDelay(125);
	stop_music();
	osDelay(157);

	play_note(NOTE_E);
	osDelay(157);
	play_note(NOTE_E);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(625);
	play_note(NOTE_A);
	osDelay(157);
	play_note(Asharp);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_DU);
	osDelay(125);
	stop_music();
	osDelay(157);

	play_note(NOTE_F);
	osDelay(157);
	play_note(NOTE_G);
	osDelay(157);
	play_note(NOTE_A);
	osDelay(625);
	play_note(NOTE_G);
	osDelay(625);
	play_note(NOTE_F);
	osDelay(157);
	play_note(NOTE_F);
	osDelay(125);

	stop_music();
	osDelay(10);
}

void play_moving_song(void) {
	play_note(NOTE_EU);
	osDelay(313);
	play_note(NOTE_B);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_DU);
	osDelay(313);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_B);
	osDelay(157);
	play_note(NOTE_A);
	osDelay(469);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_EU);
	osDelay(313);
	play_note(NOTE_DU);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_B);
	osDelay(313);
	play_note(NOTE_B);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_DU);
	osDelay(313);
	play_note(NOTE_EU);
	osDelay(313);
	play_note(NOTE_CU);
	osDelay(313);
	play_note(NOTE_A);
	osDelay(313);
	play_note(NOTE_A);
	osDelay(313);
	
/*
	stop_music();
	osDelay(500);
	play_note(NOTE_DU);
	osDelay(750);
	play_note(NOTE_FU);
	osDelay(157);
	play_note(NOTE_AU);
	osDelay(500);
	play_note(NOTE_GU);
	osDelay(157);
	play_note(NOTE_FU);
	osDelay(157);
	play_note(NOTE_EU);
	osDelay(750);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_EU);
	osDelay(500);
	play_note(NOTE_DU);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(157);
	play_note(NOTE_B);
	osDelay(500);
	play_note(NOTE_B);
	osDelay(157);
	play_note(NOTE_CU);
	osDelay(157);
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
*/

	stop_music();
	osDelay(400);
}

void play_wifi_song(void) {
	play_note(NOTE_A);
	osDelay(350);
	
	stop_music();
	osDelay(250);
	
	play_note(NOTE_A);
	osDelay(350);
	
	stop_music();
	osDelay(250);
}
