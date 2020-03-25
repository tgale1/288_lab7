/*
 * movement.h
 *
 *  Created on: Jan 29, 2020
 *      Author: jschnell
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_


double move_forward(oi_t* sens, double);
double move_backward(oi_t* sens, double);
double turn_left(oi_t* sens, double);
double turn_right(oi_t* sens, double);



#endif /* MOVEMENT_H_ */
