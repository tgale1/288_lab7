/*
 * ping.c
 *
 *  Created on: Mar 11, 2020
 *      Author: jschnell
 */

//TODO: fix ping.h
#include "ping.h"

    volatile unsigned long last_time = 0;
    volatile unsigned long current_time = 0;
    volatile int update_flag = 0; //3 states, 0,1,2
    volatile unsigned long time_mills;


void ping_send_pulse(void) {

    SYSCTL_RCGCGPIO_R |= 0b000010;
    while((SYSCTL_RCGCGPIO_R & 0b10) == 0){}
    //Disable timers by disabling AFSEL, and DIR (GPIO as out)
	
	GPIO_PORTB_AFSEL_R &= 0b0111;
	GPIO_PORTB_DIR_R |= 0b1000;

    GPIO_PORTB_DEN_R |= 0b1000;

    //GPIO_PORTB_DIR_R |= 0b1000;

    GPIO_PORTB_DATA_R &= 0;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0b1000;
    timer_waitMicros(5);


    GPIO_PORTB_DATA_R &= 0;
    timer_waitMicros(5);

    //TODO: re-enable timer, re-enable AFSEL, PCTL, and DIR (Timer is an input)
	GPIO_PORTB_AFSEL_R |= 0b1000;
	GPIO_PORTB_PCTL_R |= 0x7000;
	GPIO_PORTB_DIR_R |= 0b0111;
	
}

void timer_handler(){

    if(update_flag == 0){
	    current_time = TIMER3_TBR_R;
	    update_flag = update_flag + 1;
    }
    else if(update_flag == 1){
	    last_time = TIMER3_TBR_R;
	    update_flag = update_flag + 1;
    }
    //Clear capture interrupt flag
	TIMER3_ICR_R |= 0x400;    

}

int ping_read(void){
    ping_send_pulse();
    while(update_flag != 2){}
    // By this point we have both edges
    //find the difference between the times of both edges and return it
	update_flag = 0;
	
	
	unsigned long time_diff = current_time - last_time;
    unsigned long real_time = time_diff * 62.5; //Convert to nanoseconds
	real_time = real_time / 1000000; // Convert to milliseconds
	return real_time;
}

void ping_init_timer(void){

    SYSCTL_RCGCTIMER_R |= 0b001000;
    while((SYSCTL_RCGCTIMER_R & 0b1000) == 0){}
	
    SYSCTL_RCGCGPIO_R |= 0b000010;
    while((SYSCTL_RCGCGPIO_R & 0b000010) == 0){}

    //AFSEL, PCTL pg 1351, and DIR (Timer is an input)
	
	GPIO_PORTB_AFSEL_R |= 0b1000;
	
	GPIO_PORTB_PCTL_R |= 0x7000;

    GPIO_PORTB_DIR_R &= 0b0111;

    TIMER3_CTL_R |= 0b011111111;

    TIMER3_CFG_R |= 0x4;

    TIMER3_TBMR_R |= 0b111;

    TIMER3_CTL_R |= 0xC00;

    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;



    TIMER3_IMR_R |= 0x400;



    //activate the PING sensor

    // Clear capture interrupt flag
    TIMER3_ICR_R |= 0x400;
    // Enable event capture interrupts
    TIMER3_IMR_R |= 0x400;
    // Set up NVIC for Timer3B IC interrupts
    NVIC_EN1_R |= 0x10;
    NVIC_PRI9_R |= 0x2;
    //Bind Timer3B interrupt requests to your interrupt handler
    IntRegister(52, timer_handler);
    //enable timer
    TIMER3_CTL_R |= 0x100;
    IntMasterEnable();
}

float ping_getDistance (unsigned long time_mills){
    //calculate distance
    float distanceCM = time_mills / 1000; //Convert to seconds
    distanceCM = distanceCM * 343; // Convert to meters
    distanceCM = distanceCM * 100; // Convert to cm
    distanceCM = distanceCM / 2; //Divided by 2 to get distance

    return distanceCM;
}


int main(void) {

    timer_init();
    lcd_init();
    ping_init_timer();

    while(1){

        //call ping read here and print its return value
		
	time_mills = ping_read();
		
		
        int a = ping_getDistance();

        timer_waitMillis(100);

	lcd_printf("%lu", time_mills); 
	   
        lcd_printf("%d",a);
    }

}
