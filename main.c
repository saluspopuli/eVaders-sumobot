/* eVaders-sumobot - an application for the Pololu 3pi Robot
 *
 * This application uses the Pololu AVR C/C++ Library.  For help, see:
 * -User's guide: http://www.pololu.com/docs/0J20
 * -Command reference: http://www.pololu.com/docs/0J18
 *
 * Created: 10/16/2023 2:51:52 PM
 *  Author: Andre
 */

#include <pololu/3pi.h>

#define TRIG IO_D0
#define ECHO IO_D1

#define RIGHT 1
#define LEFT 0
#define TRUE 1
#define FALSE 0

#define BUTTON_A_CON 2
#define BUTTON_B_CON 16
#define BUTTON_C_CON 32

#define MAX_TIMEOUT 32767

// GLOBAL VARIABLES =========================================================================


// GENERAL VARS ================================================
int rammingDistance = 50;
int isRamming = FALSE;
// =============================================================


// EVASION VARS ================================================

int evasionEnableFlag = FALSE;
int evadeDirection = LEFT;

int evasionFlag = 0;
int evasionFrame = 0;

const int evadePhase2Radius = 130;
const int evadePhase3CheckSpeed = 80;

const int evadePhase1 = 10;
const int evadePhase2 = 80;
const int evadePhase3 = 200;
	
// =============================================================
	
	
	
// SPIRAL VARS =================================================

int spiralEnableFlag = FALSE;
int spiralDirection = LEFT;

int spiralFlag = 1;
int spiralMaxFrame = 2;
int tmpSpiral = 0;
int spiralSpeed = 50;

int spiralLowerLimit = 60;
int spiralUpperLimit = 180;
	
// =============================================================



// INITIAL BEHAVIOR VARS =======================================
int behavior = 1;
int initSpeed = 255;

int spinDelay = 100;
int turnDelay = 50;
// =============================================================



// ===========================================================================================

int main()
{
	play_from_program_space(PSTR(">g32>>c32"));  // Play welcoming notes.
	
	// POLULU SETUP ================================================
	set_digital_output(TRIG, LOW);
	set_digital_input(ECHO, LOW);
	
	
	const int sensorTimeOut = 2000;
	unsigned int sensorVals[5];
	
	pololu_3pi_init(sensorTimeOut);
	// =============================================================

	// DELTA LOOP VARIABLES ========================================
	int UPS = 60;

	double updateInterval = 1000/UPS;
	double delta = 0;
	long lastTime = millis();
	long currentTime;
	long timer = 0;
	int updateCount = 0;

	int deltaLoopFlag = 1;
	// =============================================================	
	
	
	// SUMOBOT SETUP ============================================================================================================
	
	if (wait_for_button_press(BUTTON_A | BUTTON_B | BUTTON_C) == BUTTON_A) {
		// A - SETUP INIT BEHAVIOR
		// B - SKIP INIT BEHAVIOR
		play_from_program_space(PSTR(">f32>>g20"));
		delay_ms(500);
		
		unsigned char buttonPress = wait_for_button_press(BUTTON_A | BUTTON_B | BUTTON_C);
		play_from_program_space(PSTR(">f32>>g20"));
		
		if (buttonPress == BUTTON_A){			// TURN LEFT
			behavior = 1;
		} else if (buttonPress == BUTTON_B){	// SPIN
			behavior = 2;
		} else if (buttonPress == BUTTON_C){	// TURN RIGHT
			behavior = 3;
		} else{
			behavior = 0;
		}
		
		delay_ms(500);
		
	} else{
		play_from_program_space(PSTR(">g20>>f32"));
		delay_ms(500);
	}
	
	// SPIRAL CHECK
	unsigned char buttonPress = wait_for_button_press(BUTTON_A | BUTTON_B | BUTTON_C);
	play_from_program_space(PSTR(">f32>>g20"));
	
	if (buttonPress == BUTTON_A){				// LEFT SPIRAL
		spiralEnableFlag = TRUE;
		spiralDirection = LEFT;
		
	} else if (buttonPress == BUTTON_B){		// DONT SPIRAL
		spiralEnableFlag = FALSE;
			
	} else if (buttonPress == BUTTON_C){		// RIGHT SPIRAL
		spiralEnableFlag = TRUE;
		spiralDirection = RIGHT;
			
	}
	delay_ms(500);
		
	// EVASION CHECK
	
	buttonPress = wait_for_button_press(BUTTON_A | BUTTON_B | BUTTON_C);
	play_from_program_space(PSTR(">f32>>g20"));
	
	if (buttonPress == BUTTON_A){				// LEFT EVADE
		evasionEnableFlag = TRUE;
		evadeDirection = LEFT;
		
	} else if (buttonPress == BUTTON_B){		// DONT EVADE
		evasionEnableFlag = FALSE;
		
	} else if (buttonPress == BUTTON_C){		// RIGHT EVADE
		evasionEnableFlag = TRUE;
		evadeDirection = RIGHT;
	}
	
	
	
	// ==========================================================================================================================
	
	play_from_program_space(PSTR(">g32"));
	delay_ms(1000);
	play_from_program_space(PSTR(">g32"));
	delay_ms(1000);
	play_from_program_space(PSTR(">g32"));
	delay_ms(1000);
	play_from_program_space(PSTR(">g32"));
	delay_ms(1000);
	play_from_program_space(PSTR(">g32"));
	delay_ms(1000);
	play_from_program_space(PSTR(">g32"));
	
	// INTIAL BEHAVIOR ==========================================================================================================
	
	switch (behavior){
		case 1:
			turnLeft(turnDelay, initSpeed);
			break;
		case 2:
			turnLeft(spinDelay, initSpeed);
			break;
		case 3:
			turnRight(turnDelay, initSpeed);
			break;
		default:
			break;
		}
			
	// ==========================================================================================================================
	
	

	
	// ======================= MAIN LOOPING FUNCTION ============================================================================
	
	while(1)
	{
		// PUT PRIORITY FUNCTIONS THAT MUST RUN EVERY CYCLE IN HERE (I.E. BORDER CHECKING) ===================
		
		// FUNCTION WHEN OBJECT IS DETECTED ===================================================
		if (ping_ultrasound() < rammingDistance){
			if (evasionEnableFlag == TRUE){
				evasionFlag = TRUE;
			} else{
				fullSpeedAhead(rammingDistance);
			}
			
			isRamming = TRUE;
			
		} else{
			isRamming = FALSE;
		}
		// ====================================================================================
		
		// BORDER CHECKING ====================================================================
		while (check_border(sensorVals,sensorTimeOut) == 1){
			int speed = line_sensor_check(sensorVals,sensorTimeOut);
			
			evasionFlag = 0;
			evasionFrame = 0;
			
			speed *= 13;
					
			if (speed > 255){
				speed = 255;
			}
			if (speed < -255){
				speed = -255;
			}
			
			if (speed < 50 && speed > 0){
				speed = 50;
			}
			
			if (speed > -50 && speed < 0){
				speed = -50;
			}
			
			if (speed < 0){
				reverse(3, 255);
				turnRight(10, speed);
			} else if (speed > 0){
				reverse(3,255);
				turnLeft(10, -speed);
			} else if (speed == 0){
				set_motors(-255, -200);
			}
		}
		// ====================================================================================
		
		
		
		// DELTA TIME LOOP FOR TIME BASED FUNCTIONS (I.E. MOVING FOR 5 SECONDS) ===============================1
		if (deltaLoopFlag == 1){
			currentTime = millis();
			delta += (currentTime - lastTime)/updateInterval;
			timer += (currentTime - lastTime);
			
			lastTime = currentTime;
			
			if (delta >= 1){
				
				// PUT DELTA TIME BASED FUNCTIONS HERE ========================================================2
				
				if (evasionFlag == TRUE){
					evasionRoutine();
						
				} else if(isRamming == TRUE){} 
					
				else {
					
					// PUT NORMAL SUMOBOT BEHAVIOR HERE ==================================================================================3
					
					evasionFrame = 0;
					
					// SPIRALING ===============
					if (spiralEnableFlag == TRUE){
						spiralRoutine(spiralDirection);
					} else {
						forward(0, 120);
					}
					// =========================
					
					// ==================================================================================================================3
				}
				
				// ============================================================================================2
				delta--;
				updateCount++; 
				// ============================================================================================1
			}
			
			if (timer >= 1000){
				updateCount = 0;
				timer = 0;
			}
		}
	}
	// ====================== MAIN LOOPING FUNCTION ===========================================================================
}

// ================== FUNCTIONS ===================================

// LINE_SENSOR_CHECK
// Returns an integer value from -20 to 20 which indicates the difference in
// IR light between the 5 on-board IR sensors (middle sensor is unused here).
// The integer range can be used to determine whether or not the robot should go
// left or right whenever it encounters uneven light.
int line_sensor_check(unsigned int *sensorVals, int sensorTimeOut ){
	read_line_sensors(sensorVals, IR_EMITTERS_ON);
	unsigned int tmp[5];
	int sum = 0, tmpDivisor;
	
	tmp[0] = -sensorVals[0];
	tmp[1] = -sensorVals[1];
	tmp[2] = 0;
	tmp[3] = sensorVals[3];
	tmp[4] = sensorVals[4];
	
	for (int i = 0; i < 5; i++){
		sum += tmp[i];
	}
	
	tmpDivisor = sensorTimeOut/10;
	sum = (sum/tmpDivisor);
	
	return sum;
}


// CHECK_BORDER
// Returns integer 1 if any of the sensors detect that there is white light or whatever
int check_border(unsigned int *sensorVals, int sensorTimeOut){
	int flag = 0;
	
	read_line_sensors(sensorVals, IR_EMITTERS_ON);
	
	for (int i = 0; i < 5; i++){
		if (sensorVals[i] < sensorTimeOut - 500){
			flag = 1;
		}
	}	
	
	return flag;	
}

// PING_ULTRASOUND
// Returns distance value based on ultrasound ping
int ping_ultrasound(){
	long timer = 0;
	int distance = 0;

	
	set_digital_output(TRIG, LOW);
	delay_us(2);
	set_digital_output(TRIG, HIGH);
	delay_us(10);
	set_digital_output(TRIG, LOW);
	
	while(!is_digital_input_high(ECHO)){
	}
	
	while(is_digital_input_high(ECHO)){
		timer++;	
	}
		
	distance = timer*0.034/2;

	return distance;
}

// EVASIONROUTINE
// Simple evasion routine whose variables are changed in global since it is frame/time based.
void evasionRoutine(){
	
	if (evasionFrame >= 0 && evasionFrame < evadePhase1){
		// EVADE PHASE 1
		if (evadeDirection == LEFT){
			turnLeft(0, 255);
		} else if (evadeDirection == RIGHT){
			turnRight(0, 255);
		}
		
	} else if (evasionFrame >= evadePhase1 && evasionFrame < evadePhase2){
		// EVADE PHASE 2
		if (evadeDirection == LEFT){
			set_motors(255, evadePhase2Radius);
		} else if (evadeDirection == RIGHT){
			set_motors(evadePhase2Radius, 255);
		}
		
	} else if (evasionFrame >= evadePhase2 && evasionFrame < evadePhase3){
		// EVADE PHASE 3
		if (evadeDirection == LEFT){
			turnRight(0, evadePhase3CheckSpeed);
		} else if (evadeDirection == RIGHT){
			turnLeft(0, evadePhase3CheckSpeed);
		}
		
		if (ping_ultrasound() < rammingDistance){
			fullSpeedAhead(rammingDistance);
			evasionFrame = evadePhase2;
		}
			
	} else if (evasionFrame >= evadePhase3){
		
		evasionFlag = 0;
		evasionFrame = 0;
	}
		
	
	
	evasionFrame++;
}

// SPIRALROUTINE
// Goes in a spiral based on direction integer, It is frame/time based
void spiralRoutine (int direction){
	if (spiralFlag == 1){
		if (tmpSpiral >= spiralMaxFrame){
			tmpSpiral = 0;
			
			if (spiralSpeed > spiralUpperLimit){
				spiralFlag = 0;
				}	else{
				spiralSpeed++;
			}
		}
		
		tmpSpiral++;
		
		} else if (spiralFlag == 0){
		if (tmpSpiral >= spiralMaxFrame){
			tmpSpiral = 0;
			
			if (spiralSpeed < spiralLowerLimit){
				spiralFlag = 1;
				}	else {
				spiralSpeed--;
			}
		}
		tmpSpiral++;
	}
	
	if (direction == RIGHT){
		set_motors(200,spiralSpeed);
	} else if (direction == LEFT){
		set_motors(spiralSpeed,200);
	}
}

// =================== MOVEMENT FUNCTIONS ===========================
// speed can only be between 0-255
//
// delay is only for millisecond delay after executing the movement, it can
// just be zero.

void fullSpeedAhead (int distance){
	if (ping_ultrasound() < distance){
		set_motors(255,255);
		delay_ms(5);
	}
}

void forward(int delay, int speed){
	set_motors(speed, speed);
	delay_ms(delay);
}

void reverse(int delay, int speed){
	set_motors(-speed, -speed);
	delay_ms(delay);
}

void turnLeft(int delay, int speed){
	set_motors(-speed, speed);
	delay_ms(delay);
}

void turnRight(int delay, int speed){
	set_motors(speed, -speed);
	delay_ms(delay);
}

void turnLeftBig(int delay, int speed){
	set_motors(0, speed);
	delay_ms(delay);
}

void turnRightBig(int delay, int speed){
	set_motors(speed,0);
	delay_ms(delay);
}
