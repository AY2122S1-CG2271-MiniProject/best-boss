#include "commonHeads.h"
#include "bossMotors.h"

void InitMotor(void) {
  // Enable Clock Gating for PORTB and PORTD
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
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
	
	//Configure Mode for Motor Front Back Control;
	PORTC->PCR[FRONT_PIN] &= ~PORT_PCR_MUX_MASK;
  PORTC->PCR[FRONT_PIN] |= PORT_PCR_MUX(3);
	
	PORTC->PCR[REAR_PIN] &= ~PORT_PCR_MUX_MASK;
  PORTC->PCR[REAR_PIN] |= PORT_PCR_MUX(3);
	
  // Enable Clock Gating for Timer0 and Timer2
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

  // Select clock for TPM module
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // MCGCLLCLK or  MCGLLCLK/2

  // Set Modulo Value 20971520 / 128 = 163840 / 3276 = 50 Hz
  TPM0->MOD = MOTOR_FAST; //use most reliable movement direction	

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

  // Enable PWM on TPM0 Channel 4 -> PTC8
  TPM0_C4SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

  // Enable PWM on TPM0 Channel 5 -> PTC9
  TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void FrontReverseRear(void) {
	TPM0_C4V = 0;
	TPM0_C5V = MOTOR_FAST;
}

///*
void motor_control(enum move_t move) {
	// 0: left reverse, 1: left forwards, 2: right reverse, 3: right forwards
	switch(move) {
		case STOP:
			TPM0_C0V = 0;															//RIGHT_FRONT0
      TPM0_C1V = 0;															//RIGHT_REAR1
      TPM0_C2V = 0;															//LEFT_FRONT2
      TPM0_C3V = 0;															//LEFT_REAR3
			break;
		case TURN_LEFT: // Rotate Left
			FrontReverseRear();
			TPM0_C0V = MOTOR_FAST;										//RIGHT_FRONT yes
      TPM0_C1V = MOTOR_FAST;															//RIGHT_REAR no
      TPM0_C2V = 0;										//LEFT_FRONT no
      TPM0_C3V = 0;															//LEFT_REAR yes
			break;
		case TURN_RIGHT: // Rotate Right
			FrontReverseRear();
			TPM0_C0V = 0;															//RIGHT_FRONT
			TPM0_C1V = 0;															//RIGHT_REAR
			TPM0_C2V = MOTOR_FAST;										//LEFT_FRONT
			TPM0_C3V = MOTOR_FAST;										//LEFT_REAR
			break;
		
		case FORWARD: // Forward
      // Switch on RED LED
			TPM0_C0V = MOTOR_FAST;
			TPM0_C1V = MOTOR_FAST; 
			TPM0_C2V = MOTOR_FAST;
			TPM0_C3V = MOTOR_FAST;
			break;
		case CURVE_LEFT: // Forward + Left
			TPM0_C0V = MOTOR_FAST;									//RIGHT_FRONT
			TPM0_C1V = MOTOR_SLOW;											//RIGHT_REAR
			TPM0_C2V = 0;						//LEFT_FRONT
			TPM0_C3V = MOTOR_SLOWER;								//LEFT_REAR
			break;
		case CURVE_RIGHT: // Forward + Right
			TPM0_C0V = 0;
			TPM0_C1V = MOTOR_SLOWER;
			TPM0_C2V = MOTOR_FAST;
			TPM0_C3V = MOTOR_SLOW;
			break;
		
		case REVERSE: // Backward
			TPM0_C4V = MOTOR_FAST;
			TPM0_C5V = MOTOR_FAST;

			TPM0_C0V = 0;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 0;
			break;
		case REVERSE_LEFT: // Rotate LEFT BACKWARDS
			TPM0_C4V = MOTOR_FAST;
			TPM0_C5V = MOTOR_FAST;

			TPM0_C0V = 0;						//RIGHT_FRONT
			TPM0_C1V = 0;						//RIGHT_REAR
			TPM0_C2V = 0;						//LEFT_FRONT
			TPM0_C3V = MOTOR_FAST;						//LEFT_REAR
			break;
		case REVERSE_RIGHT: // Rotate Right BACKWARDS
			TPM0_C4V = MOTOR_FAST;
			TPM0_C5V = MOTOR_FAST;
		
			TPM0_C0V = 0;
			TPM0_C1V = MOTOR_FAST;
			TPM0_C2V = 0;
			TPM0_C3V = 0;
			break;
	}
}

void forward(void) {
	TPM0_C4V = 0;
	TPM0_C5V = 0;
}

void reverse(void) {
	TPM0_C4V = MOTOR_FAST;
	TPM0_C5V = MOTOR_FAST;
}

void stop(void) {
	forward();
	driveInstructions = 0x00;
	motor_control(STOP);
}

void rewrite_driveMode(uint8_t optionNumber) {
	driveInstructions = 0x00;
	NorthSouth = DRIVE_BACK_GO(optionNumber);
	driveInstructions = MERGE_INSTRUCTIONS(NorthSouth, EastWest);
	switch (optionNumber) {
		case USER_STOP:		
			forward();
			driveInstructions = STOP;
			break;
		case USER_FORWARD:
			forward();
			break;
		case USER_REVERSE:
			reverse();
			break;
		default:
			//flashRedLight
			stop();
			break;
	}
}

void rewrite_direction(uint8_t optionNumber) {
	driveInstructions = 0x00;
	EastWest = DRIVE_WAY(optionNumber);
	driveInstructions = MERGE_INSTRUCTIONS(EastWest, NorthSouth);
	switch (optionNumber) {
		case USER_TURN_LEFT:
			break;
		case USER_TURN_RIGHT:
			break;
		case USER_STRAIGHT:
			break;
		default:
			//flashRedLight
			stop();
			break;
	}
}

void executeDrive(uint8_t instructions) {
	enum move_t move;
	switch (instructions) {
		case STAYSTILL:
			stop();
			break;
		case FORWARD_STRAIGHT:
			move = FORWARD;
			break;
		case FORWARD_LEFT:
			move = CURVE_LEFT;
			break;
		case FORWARD_RIGHT:
			move = CURVE_RIGHT;
			break;
		case BACKWARD_STRAIGHT:
			move = REVERSE;
			break;
		case BACKWARD_LEFT:
			move = REVERSE_LEFT;
			break;
		case BACKWARD_RIGHT:
			move = REVERSE_RIGHT;	
			break;
		case STILL_LEFT:
			move = TURN_LEFT;
			break;
		case STILL_RIGHT:
			move = TURN_RIGHT;
			break;
		default:
			move = STOP;
			break;
	}
	motor_control(move);
}
