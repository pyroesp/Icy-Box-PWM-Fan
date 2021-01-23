/*
 * File:   main.c
 * Author: pyroesp
 * 
 * Icy Box fan control for PIC16F18325
 *
 * Created on September 21, 2019, 5:33 PM
 
 Connection:
    RC1 = DATA : ANC1
    RC5 = RST : PWM
 
 Fan size : 80x80mm
 */

#include "mcc_generated_files/mcc.h"

#define _BV(b) (1<<(b))



void main(void){
    uint16_t adc_result;
    uint16_t pwm_duty_cycle;
    SYSTEM_Initialize();
    TMR2_StartTimer();
    
    pwm_duty_cycle = PWM5_INITIALIZE_DUTY_VALUE;
    PWM5_LoadDutyValue(pwm_duty_cycle);
    for(;;){
        // ADC is 10 bits, mask the 10 bits just in case
        adc_result = ADC_GetConversion(channel_ANC1);
        adc_result &= 0x03FF;
        // PWM duty cycle is is 8 bits, so shift adc value right by 2
        adc_result >>= 2;
        // check PWM duty cycle, if too low set minimum duty cycle
        if (adc_result < PWM5_INITIALIZE_DUTY_VALUE)
            adc_result = PWM5_INITIALIZE_DUTY_VALUE;
        // if too high set to maximum
        else if (adc_result > 0xFF)
            adc_result = 0xFF;
        PWM5_LoadDutyValue(adc_result);
        
        __delay_ms(1000);
    }
    
    for(;;);
}