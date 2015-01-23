/*
 *  P1-12 输出PWM波
 *  频率 1171.875Hz
 *  占空比 0-100% 范围内周期性变化
 *  
 *  Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *  Copyright (C) 2014 w.guihong <w.guihong@openrpi.org>
 *  Copyright (C) 2014 openRPi <www.openrpi.org>
 *  
 *      代码遵循GNU协议
 */

#include <stdio.h>
#include "bcm2835.h"
#include <signal.h>

// PWM output on RPi Plug P1 pin 12 (which is GPIO pin 18)
// in alt fun 5.
// Note that this is the _only_ PWM pin available on the RPi IO headers
#define PIN RPI_GPIO_P1_12
// and it is controlled by PWM channel 0
#define PWM_CHANNEL 0
// This controls the max range of the PWM signal
#define RANGE 1024

int loop_run = 1;

void loop_stop(int sig)
{
	loop_run = 0;
}

int main(int argc, char **argv)
{
    if (!bcm2835_init())
    {
		printf("GPIO init ERROR\n");
        return 1;
    }
    
    // Set the signal
	signal(SIGINT,loop_stop); // Ctrl-C
    
    // Set the output pin to Alt Fun 5, to allow PWM channel 0 to be output there
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_ALT5);
    
    // Clock divider is set to 16.
    // With a divider of 16 and a RANGE of 1024, in MARKSPACE mode,
    // the pulse repetition frequency will be
    // 1.2MHz/1024 = 1171.875Hz, suitable for driving a DC motor with PWM
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
    
    printf("PWM on P1-12, %fHz\n",1200000.0/RANGE);
    
    // Vary the PWM m/s ratio between 1/RANGE and (RANGE-1)/RANGE
    int direction = 1;
    int data = 1;
    while (loop_run)
	{
		if (data == 1)
			direction = 1;
		else if (data == RANGE-1)
			direction = -1;
		data += direction;
		bcm2835_pwm_set_data(PWM_CHANNEL, data);
		printf("\rDuty: %f",(float)data/RANGE);
		bcm2835_delay(10);
	}
	
	bcm2835_pwm_set_data(PWM_CHANNEL,0);
    bcm2835_close();
    printf("Exit\n");
    return 0;
}
