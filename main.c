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

<<<<<<< Updated upstream
=======
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
int rammingDistance = 80;
int isRamming = FALSE;

// =============================================================


// EVASION VARS ================================================

int evasionEnableFlag = FALSE;
int evadeDirection = LEFT;

int evasionFlag = 0;
int evasionFrame = 0;

const int evadePhase2Radius = 150;
const int evadePhase3CheckSpeed = 80;

const int evadePhase1 = 13;
const int evadePhase2 = 80;
const int evadePhase3 = 150;
	
// =============================================================
	
	
	
// SPIRAL VARS =================================================

int spiralEnableFlag = FALSE;
int spiralDirection = LEFT;

int spiralFlag = 1;
int spiralMaxFrame = 2;
int tmpSpiral = 0;
int spiralSpeed = 50;

int spiralSwitchFrame = 0;
int spiralMaxSwitchFrame = 240;
int spiralSwitchFlag = FALSE;
int spiralSwitchCountdownFlag = FALSE;
int spiralSwitchLimit = 5;
int currSpiralSwitchLimit = 0;

int spiralLowerLimit = 60;
int spiralUpperLimit = 150;
	
// =============================================================



// INITIAL BEHAVIOR VARS =======================================
int behavior = 1;
int initSpeed = 255;

int spinDelay = 220;
int turnDelay = 180;
// =============================================================



// ===========================================================================================

>>>>>>> Stashed changes
int main()
{
	play_from_program_space(PSTR(">g32>>c32"));  // Play welcoming notes.
	
	// POLULU SETUP ================================================
	int length = 1000;
	int speed = 100;
	
	const int sensorTimeOut = 2000;
	unsigned int sensorVals[5];
	
	pololu_3pi_init(sensorTimeOut);
	// POLULU SETUP ================================================
	
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
	
	// EVASION VARS ================================================
	
	int evasionFlag = 0;
	int evasionFrame = 0;
	
	
	int testFrames = 0;
	int maxTestFrames = 100;
	
	const int evadePhase1 = 4;
	const int evadePhase2 = 50;
	const int evadePhase3 = 200;
	
	// =============================================================
	
	// ======================= MAIN LOOPING FUNCTION ============================================================================
	while(1)
	{
		// PUT PRIORITY FUNCTIONS THAT MUST RUN EVERY CYCLE IN HERE (I.E. BORDER CHECKING) ===================
		
		
		// BORDER CHECKING ====================================================================
		while (check_border(sensorVals,sensorTimeOut) == 1){
			int speed = line_sensor_check(sensorVals,sensorTimeOut);
			
			clear();
			lcd_goto_xy(0,0);
			
			spiralSwitchFlag = TRUE;
			
			speed *= 13;
			print_long(speed);
			
			if (speed > 255){
				speed = 255;
				
			}
			
			if (speed < 0){
				turnRight(10, speed);
			} else if (speed > 0){
				turnLeft(10, -speed);
			} else if (speed == 0){
				reverse(10,150);
			}
		}
		
		// ====================================================================================
		
		
		
		// DELTA TIME LOOP FOR TIME BASED FUNCTIONS (I.E. MOVING FOR 5 SECONDS) ===============================
		if (deltaLoopFlag == 1){
			currentTime = millis();
			delta += (currentTime - lastTime)/updateInterval;
			timer += (currentTime - lastTime);
			
			lastTime = currentTime;
			
			if (delta >= 1){
				
				// PUT DELTA TIME BASED FUNCTIONS HERE ========================================================
				
				// EVASION AND RAMMING ===============================================================
				
				if (evasionFlag == 1){
					
					if (evasionFrame >= 0 && evasionFrame < evadePhase1){
						turnLeft(0, 255);
					} else if (evasionFrame >= evadePhase1 && evasionFrame < evadePhase2){
						set_motors(255, 130);
					} else if (evasionFrame >= evadePhase2 && evasionFrame < evadePhase3){
						turnRight(0, 50);
					} else if (evasionFrame >= evadePhase3){
						evasionFlag = 0;
					}
					
					evasionFrame++;
					
				} else{
					evasionFrame = 0;
					
					if (testFrames < maxTestFrames){
						forward(0,100);
						testFrames++;
					} else {
<<<<<<< Updated upstream
						testFrames = 0;
						evasionFlag=1;
=======
						forward(0, 160);
>>>>>>> Stashed changes
					}
				}
				
				// ===================================================================================
				
				
				
				
				// ============================================================================================
				delta--;
				updateCount++; 
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
	int sum, tmpDivisor;
	
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

int check_border(unsigned int *sensorVals, int sensorTimeOut){
	int flag = 0;
	
	read_line_sensors(sensorVals, IR_EMITTERS_ON);
	
	for (int i = 0; i < 5; i++){
		if (sensorVals[i] < sensorTimeOut - 200){
			flag = 1;
		}
	}	
	
	return flag;	
}

<<<<<<< Updated upstream
=======
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
	
	while(!is_digital_input_high(ECHO)){}
	
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
		if (ping_ultrasound() < rammingDistance){
			fullSpeedAhead(rammingDistance);
			evasionFrame = evadePhase2;
		} else{
			if (evadeDirection == LEFT){
				turnRight(0, evadePhase3CheckSpeed);
			} else if (evadeDirection == RIGHT){
				turnLeft(0, evadePhase3CheckSpeed);
		}
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
	
	if (spiralSwitchFlag == TRUE){
		spiralSwitchFrame = 0;
		spiralSwitchFlag = FALSE; 
		spiralSwitchCountdownFlag = TRUE;
		currSpiralSwitchLimit++;
	}
	
	if (spiralSwitchCountdownFlag == TRUE){
		if (spiralSwitchFrame > spiralMaxSwitchFrame){
			spiralSwitchFrame = 0;
			spiralSwitchCountdownFlag = FALSE;
			currSpiralSwitchLimit = 0;
		} else{
			spiralSwitchFrame++;
		}
	}
	
	if (currSpiralSwitchLimit >= spiralSwitchLimit){
		currSpiralSwitchLimit = FALSE;
		spiralSwitchCountdownFlag = FALSE;
		if (spiralDirection == LEFT){
			spiralDirection = RIGHT;
		} else if (spiralDirection == RIGHT){
			spiralDirection = LEFT;
		}
	}
}
>>>>>>> Stashed changes

// =================== MOVEMENT FUNCTIONS ===========================
// speed can only be between 0-255
//
// delay is only for millisecond delay after executing the movement, it can
// just be zero.

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
