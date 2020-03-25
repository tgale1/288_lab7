/*
 * ping.h
 *
 *  Created on: Mar 11, 2020
 *      Author: jschnell
 */

#ifndef PING_H_
#define PING_H_


void ping_init (void); //initialize the timer
void ping_trigger (void); //activate the PING sensor
float ping_getDistance (void); //calculate distance
void timer_handler(void);


#endif /* PING_H_ */
