#include "commonHeads.h"
#include "bossMotors.h"

void InitMotor(void) {
  // Enable Clock Gating for PORTB and PORTD
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
  SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

  // Configure Mode 4 for motor PWM
  PORTD->PCR[PTD0_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD0_Pin] |= PORT_PCR_MUX(4);

  PORTD->PCR[PTD1_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD1_Pin] |= PORT_PCR_MUX(4);

  PORTD->PCR[PTD2_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD2_Pin] |= PORT_PCR_MUX(4);

  PORTD->PCR[PTD3_Pin] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD3_Pin] |= PORT_PCR_MUX(4);

  // Enable Clock Gating for Timer0 and Timer1
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

  // Select clock for TPM module
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // MCGCLLCLK or  MCGLLCLK/2

  // Set Modulo Value 20971520 / 128 = 163840 / 3276 = 50 Hz
  TPM0->MOD = MOTOR_FAST_LEFT; //use most reliable movement direction

  /* Edge-Aligned PWM */
  // Update SnC register: CMOD = 01, PS = 111 (128)
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK);

  // Enable PWM on TPM0 Channel 0 -> PTD0
  TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

  // Enable PWM on TPM0 Channel 1 -> PTD1
  TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

  // Enable PWM on TPM0 Channel 2 -> PTD2
  TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

  // Enable PWM on TPM0 Channel 3 -> PTD3
  TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

  // Enable PWM on TPM1 Channel 0 -> PTB0
  TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void motor_control(enum move_t move) {
	// 0: left reverse, 1: left forwards, 2: right reverse, 3: right forwards
	switch(move) {
		case STOP:
			TPM0_C0V = 0;
      TPM0_C1V = 0;
      TPM0_C2V = 0;
      TPM0_C3V = 0;
			break;
		case FORWARD: // Forward
			TPM0_C0V = 0;
			TPM0_C1V = MOTOR_FAST_LEFT; 
			TPM0_C2V = 0;
			TPM0_C3V = MOTOR_FAST_RIGHT;
			break;
		case CURVE_LEFT: // Forward + Left
			TPM0_C0V = 0;
			TPM0_C1V = MOTOR_SLOWER_LEFT;
			TPM0_C2V = 0;
			TPM0_C3V = MOTOR_FAST_RIGHT;
			break;
		case CURVE_RIGHT: // Forward + Right
			TPM0_C0V = 0;
			TPM0_C1V = MOTOR_FAST_LEFT;
			TPM0_C2V = 0;
			TPM0_C3V = MOTOR_SLOWER_RIGHT;
			break;
		case REVERSE: // Backward
			TPM0_C0V = MOTOR_FAST_LEFT;
			TPM0_C1V = 0;
			TPM0_C2V = MOTOR_FAST_RIGHT;
			TPM0_C3V = 0;
			break;
		case TURN_LEFT: // Rotate Left
			TPM0_C0V = MOTOR_SLOW_LEFT;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = MOTOR_SLOW_RIGHT;
			break;
		case TURN_RIGHT: // Rotate Right
			TPM0_C0V = 0;
			TPM0_C1V = MOTOR_SLOW_LEFT;
			TPM0_C2V = MOTOR_SLOW_RIGHT;
			TPM0_C3V = 0;
			break;
	}
}