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

int main()
{
	play_from_program_space(PSTR(">g32>>c32"));  // Play welcoming notes.
	
	// POLULU SETUP ================================================
	int length = 1000;
	int turnSpeed = 100;
	unsigned int sensorVals[5];
	
	pololu_3pi_init(2000);
	// POLULU SETUP ================================================
	
	while(1)
	{
		turnLeftBig(500, 100);
		turnRightBig(500, 100);
	}
}

// ================== FUNCTIONS ===================================

int line_sensor_check(unsigned int *sensorVals){
	read_line_sensors(sensorVals, IR_EMITTERS_ON);
	
	// Traverse a 5 element array
}


// =================== MOVEMENT FUNCTIONS ===========================

void turnLeft(int delay, int turnSpeed){
	set_motors(-turnSpeed, turnSpeed);
	delay_ms(delay);
}

void turnRight(int delay, int turnSpeed){
	set_motors(turnSpeed, -turnSpeed);
	delay_ms(delay);
}

void turnLeftBig(int delay, int turnSpeed){
	set_motors(0, turnSpeed);
	delay_ms(delay);
}

void turnRightBig(int delay, int turnSpeed){
	set_motors(turnSpeed,0);
	delay_ms(delay);
}
