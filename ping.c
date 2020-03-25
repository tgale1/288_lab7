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


void ping_send_pulse (void) {

    //TODO: Disable timers by disabling AFSEL, and DIR (GPIO as out)

    SYSCTL_RCGCGPIO_R |= 0b000010;
    while((SYSCTL_RCGCGPIO_R & 0b10) == 0){}

    GPIO_PORTB_DEN_R |= 0b1000;

    GPIO_PORTB_DIR_R |= 0b1000;

    GPIO_PORTB_DATA_R &= 0;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0b1000;
    timer_waitMicros(5);


    GPIO_PORTB_DATA_R &= 0;
    timer_waitMicros(5);

    //TODO: re-enable timer, re-enable AFSEL, PCTL, and DIR (Timer is an input)
}

void timer_handler(){

    /* TODO:
     * if (state is 0)
     *      grab the rising time
     *      advance state
     * else if (state is 1)
     *      grab the falling time
     *      advance state
     * clear the interrupt;
     *
     */
    last_time = current_time;
    current_time = TIMER1_TAR_R; //TODO: we are using timer B here, not A
    update_flag = 1;


}

int ping_read(void){
    ping_send_pulse();
    while( ){} //TODO: Busy wait until state is equal to 2
    // By this point we have both edges
    //TODO: find the difference between the times of both edges and return it

}

void ping_init_timer (void){

    SYSCTL_RCGCTIMER_R |= 0b001000;
    while((SYSCTL_RCGCTIMER_R & 0b1000) == 0){}

    //TODO: AFSEL, PCTL pg 1351, and DIR (Timer is an input)

    GPIO_PORTB_DIR_R &= 0;

    TIMER3_CTL_R = 0; //TODO: turn it off properly TBEN

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
    NVIC_EN3_R |= 0x20;         //TODO: incorrect EN register
    NVIC_PRI25_R |= 0x2000;     //TODO: incorrect PRI register
    //Bind Timer3B interrupt requests to your interrupt handler
    IntRegister(52, timer_handler);
    //enable timer
    TIMER3_CTL_R |= 0x100;
    IntMasterEnable();
}

float ping_getDistance (void){
    //calculate distance
    unsigned long time_diff = current_time - last_time;
    unsigned long real_time = time_diff/32000000;
    time_mills = real_time * 1000;
    float distanceCM = real_time * 34300;

    return distanceCM;
}


int main(void) {

    timer_init();
    lcd_init();
    ping_init();
    ping_trigger();

    while(1){

        // TODO: call ping read here and print its return value
        int a = TIMER3_TBV_R;

        timer_waitMillis(100);

        lcd_printf("%d",a);
    }

}
