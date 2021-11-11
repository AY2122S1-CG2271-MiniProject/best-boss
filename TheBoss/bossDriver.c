#include "bossDriver.h"

#define STEPS 20
#define MAX_DUTY_CYCLE 0x1D4C //7500 (50Hz)
volatile int j[STEPS] = {0};
volatile int k[STEPS] = {0};


//RX_data Processing Tools
uint8_t driveInstructions, NorthSouth, EastWest;

void initUltrasonic(void) {

	//Enable clock gating for portD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTA_MASK) | (SIM_SCGC5_PORTD_MASK));

	//Set TRIG_PIN as GPIO output to trigger the sensor
	PORTD->PCR[TRIG_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[TRIG_PIN] |= PORT_PCR_MUX(1);
	PTD->PDDR |= MASK(TRIG_PIN);

	//Set ECHO_PIN pin to TPM1_CH1 mode and pin to input
	PORTA->PCR[ECHO_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[ECHO_PIN] |= (PORT_PCR_MUX(3));
	PTA->PDDR &= ~(MASK(ECHO_PIN));

	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	//Select Clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	// 375000 / 7500 = 50 Hz [basically when reach 7500 (max count) will flip over to 0 and start again]
	TPM1->MOD = 7500;


	//Edge-Aligned PWM
  //CMOD - 1 and PS - 111 (128)
	TPM1_SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1_SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7)); //CMOD = 1 => LPTPM counter increments on every LPTPM counter clock
	TPM1_SC &= ~(TPM_SC_CPWMS_MASK); //count up by default (0)

	  //Configure channel 3 from TPM0 as input capture for both edges and enabling interrupt
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_CHIE_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(1) | TPM_CnSC_CHIE(1));

	//Enable TPM0 interrupt
	NVIC_SetPriority(TPM1_IRQn, 2);
	NVIC_ClearPendingIRQ(TPM1_IRQn);
	NVIC_EnableIRQ(TPM1_IRQn);

	//Disable Clk to avoid interrupt triggers
	SIM->SCGC6 &= ~SIM_SCGC6_TPM1_MASK;

}


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

	// Configure Mode for Motor Front Back Control;
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

void leftReverse(void) {
	TPM0_C4V = 0;
	TPM0_C5V = MOTOR_FAST;
}

void rightReverse(void) {
	TPM0_C4V = MOTOR_FAST;
	TPM0_C5V = 0;
}

void forward(void) {
	TPM0_C4V = 0;
	TPM0_C5V = 0;
}

void reverse(void) {
	TPM0_C4V = MOTOR_FAST;
	TPM0_C5V = MOTOR_FAST;
}

///*
void motor_control(enum move_t move) {
	// 0: left reverse, 1: left forwards, 2: right reverse, 3: right forwards
	switch (move) {
	case STOP:
		TPM0_C0V = 0;												//RIGHT_FRONT0
		TPM0_C1V = 0;												//RIGHT_REAR1
		TPM0_C2V = 0;												//LEFT_FRONT2
		TPM0_C3V = 0;												//LEFT_REAR3
		break;
	case TURN_LEFT: // Rotate Left
		leftReverse();		// or rightForwardLeftReverse() (right 0;left high)
		TPM0_C0V = MOTOR_FAST;										//RIGHT_FRONT yes
		TPM0_C1V = MOTOR_FAST;										//RIGHT_REAR no
		TPM0_C2V = 0;												//LEFT_FRONT no
		TPM0_C3V = 0;												//LEFT_REAR yes
		break;
	case TURN_RIGHT: // Rotate Right
		rightReverse();  // or rightForwardLeftReverse()
		TPM0_C0V = 0;												//RIGHT_FRONT
		TPM0_C1V = 0;												//RIGHT_REAR
		TPM0_C2V = MOTOR_FAST;										//LEFT_FRONT
		TPM0_C3V = MOTOR_FAST;										//LEFT_REAR
		break;
		//case TURN_LEFT: // Rotate Left
		//	RightForwardLeftReverse();
		//	TPM0_C0V = MOTOR_FAST;										//RIGHT_FRONT yes
		//	TPM0_C1V = MOTOR_FAST;										//RIGHT_REAR yes
		//	TPM0_C2V = 0;												//LEFT_FRONT yes
		//	TPM0_C3V = 0;												//LEFT_REAR yes
		//	break;
		//case TURN_RIGHT: // Rotate Right
		//	RightFowardLeftReverse();
		//	TPM0_C0V = 0;												//RIGHT_FRONT
		//	TPM0_C1V = 0;												//RIGHT_REAR
		//	TPM0_C2V = MOTOR_FAST;										//LEFT_FRONT
		//	TPM0_C3V = MOTOR_FAST;										//LEFT_REAR
		//	break;

	case FORWARD: // Forward
  // Switch on RED LED
		TPM0_C0V = MOTOR_FAST;
		TPM0_C1V = MOTOR_FAST;
		TPM0_C2V = MOTOR_FAST;
		TPM0_C3V = MOTOR_FAST;
		break;
	case CURVE_LEFT: // Forward + Left
		TPM0_C0V = MOTOR_FAST;										//RIGHT_FRONT
		TPM0_C1V = MOTOR_SLOW;										//RIGHT_REAR
		TPM0_C2V = 0;												//LEFT_FRONT
		TPM0_C3V = MOTOR_SLOWER;									//LEFT_REAR
		break;
	case CURVE_RIGHT: // Forward + Right
		TPM0_C0V = 0;
		TPM0_C1V = MOTOR_SLOWER;
		TPM0_C2V = MOTOR_FAST;
		TPM0_C3V = MOTOR_SLOW;
		break;

	case REVERSE: // Backward
		reverse();
		TPM0_C0V = 0;
		TPM0_C1V = 0;
		TPM0_C2V = 0;
		TPM0_C3V = 0;
		break;
	case REVERSE_LEFT: // Rotate LEFT BACKWARDS
		reverse();
		TPM0_C0V = 0;												//RIGHT_FRONT
		TPM0_C1V = 0;												//RIGHT_REAR
		TPM0_C2V = 0;												//LEFT_FRONT
		TPM0_C3V = MOTOR_FASTER;							//LEFT_REAR
		break;
	case REVERSE_RIGHT: // Rotate Right BACKWARDS
		reverse();
		TPM0_C0V = 0;
		TPM0_C1V = MOTOR_FASTER;
		TPM0_C2V = 0;
		TPM0_C3V = 0;
		break;
	}
}

void stop(void) {
	forward();
	driveInstructions = 0x00;
	motor_control(STOP);
}

void rewrite_driveMode(uint8_t optionNumber) {
	NorthSouth = DRIVE_BACK_GO(optionNumber);
	driveInstructions = MERGE_INSTRUCTIONS(EastWest, NorthSouth);
	/*switch (optionNumber) {
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
	}*/
}

void rewrite_direction(uint8_t optionNumber) {
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

void executeDrive(void) {
	enum move_t move;
	switch (driveInstructions) {
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

int isDriving(void) {
	return (driveInstructions == STAYSTILL) ? 0 : 1;
}

int auto_modeOn = 0;
void handleAutoSwitch(uint8_t option) {
	auto_modeOn = (option == USER_AUTO) ? 1 : ((option == END_AUTO) ? 0 : auto_modeOn);
}

void driverless_mode() {
	driveInstructions = STAYSTILL;
	stop();
	osDelay(225);
	
	driveInstructions = STILL_LEFT;
	motor_control(TURN_LEFT);
	osDelay(225);
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = FORWARD_STRAIGHT;
	motor_control(FORWARD);
	osDelay(500);
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = STILL_RIGHT;
	motor_control(TURN_RIGHT);
	osDelay(500);
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = FORWARD_STRAIGHT;
	motor_control(FORWARD);
	osDelay(500);
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = STILL_RIGHT;
	motor_control(TURN_RIGHT);
	osDelay(600); //2nd right
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = FORWARD_STRAIGHT;
	motor_control(FORWARD);
	osDelay(500);
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = STILL_RIGHT;
	motor_control(TURN_RIGHT);
	osDelay(500); 
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = FORWARD_STRAIGHT;
	motor_control(FORWARD);
	osDelay(500);
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	driveInstructions = STILL_LEFT;
	motor_control(TURN_LEFT);
	osDelay(400); //need to turn more
	driveInstructions = STAYSTILL;
	stop();
	osDelay(500);
	
	
	//forward until 
}
