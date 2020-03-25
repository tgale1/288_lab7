/*
 * movement.c
 *
 *  Created on: Jan 29, 2020
 *      Author: jschnell
 */
#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "string.h"
#include "movement.h"

double move_forward(oi_t *sensor_data, double distance_mm){
    //char sumArray[sizeof(distance_mm)];
    double sum = 0; // distance member in oi_t struct is type double
    double reverse = -150;
    double turn = 80;
    double lateral = 250;

    oi_setWheels(300,300); //move forward at full speed

    while (sum <= distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        //memcpy(sumArray,&sum,sizeof(distance_mm));
        //lcd_printf("%2.0lf",sum);

        if(sensor_data->bumpLeft){
            move_backward(sensor_data, reverse);
            sum += reverse;
            turn_right(sensor_data, turn);
            move_forward(sensor_data, lateral);
            turn_left(sensor_data, turn);
            move_forward(sensor_data, distance_mm - sum);
        }
        else if(sensor_data->bumpRight){
            move_backward(sensor_data, reverse);
            sum += reverse;
            turn_left(sensor_data, turn);
            move_forward(sensor_data, lateral);
            turn_right(sensor_data, turn);
            move_forward(sensor_data, distance_mm - sum);
        }
    }
    oi_setWheels(0,0); //stop
    return sum;
}


double move_backward(oi_t *sensor_data, double distance_mm){
    //char sumArray[sizeof(distance_mm)];
    double sum = 0; // distance member in oi_t struct is type double

    oi_setWheels(-300,-300); //move forward at full speed

    while (sum >= distance_mm) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        //memcpy(sumArray,&sum,sizeof(distance_mm));
        //lcd_printf("%2.0lf",sum);

    }
    oi_setWheels(0,0); //stop
    return sum;
}

double turn_left(oi_t *sensor_data, double degrees){

    double sum = 0;
    oi_setWheels(250,-250);

    while (sum <= degrees) {

        sum += sensor_data -> angle;
        oi_update(sensor_data);
    }

    oi_setWheels(0,0);
    return sum;
}

double turn_right(oi_t *sensor_data, double degrees){

    double sum = 0;
    oi_setWheels(-250,250);

    while (sum <= degrees) {


        sum -= sensor_data -> angle;
        oi_update(sensor_data);
    }

    oi_setWheels(0,0);
    return sum;
}
