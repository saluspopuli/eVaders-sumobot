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
	
	const int sensorTimeOut = 2000;
	unsigned int sensorVals[5];
	
	pololu_3pi_init(sensorTimeOut);
	// POLULU SETUP ================================================
	
	while(1)
	{
		clear();
		lcd_goto_xy(0,0);
		print_long(line_sensor_check(sensorVals,sensorTimeOut));
		delay_ms(1000);
	}
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
	sum = (sum/tmpDivisor)-1;
	
	return sum;
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
