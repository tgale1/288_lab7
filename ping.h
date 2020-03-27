/*
 * ping.h
 *
 *  Created on: Mar 11, 2020
 *      Author: jschnell
 */

#ifndef PING_H_
#define PING_H_

void ping_init_timer (void); //Initializes the timer
void ping_send_pulse (void); //Sends the initial pulse
int ping_read (void); //Reads the data incoming from the ping sensor
float ping_getDistance (void); //calculate distance from the data from the ping sensor
void timer_handler(void); //Handles interrupts in the timer


#endif /* PING_H_ */
