/*
 * File:   lab7.c
 * Author: Santiago
 *
 * Created on March 31, 2023
 */

#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = OFF

#pragma config BOR4V = BOR40V
#pragma config WRT = OFF
        
#include <xc.h>
#include <stdint.h>
#include "pwm_config.h"

#define _XTAL_FREQ 8000000

uint8_t cont;
uint8_t valp3;
char canal1;
char canal2;

void setup(void);

void __interrupt() isr(void){
    if (PIR1bits.ADIF){
        if (ADCON0bits.CHS == 0b0000){
            /*CCPR1L = (ADRESH>>1)+124;
            CCP1CONbits.DC1B1 = ADRESH & 0b01;
            CCP1CONbits.DC1B0 = ADRESL >> 7;*/
            canal1="A";
            PWM_config(canal1, 1000);
            PWM_duty(canal1, 100);
            
        }
        else if(ADCON0bits.CHS == 0b0001){/*
            CCPR2L = (ADRESH>>1)+124;
            CCP2CONbits.DC2B1 = ADRESH & 0b01;
            CCP2CONbits.DC2B0 = ADRESL >> 7;*/
            canal2="B";
            PWM_config(canal2, 1000);
            PWM_duty(canal2, 100);
        }
        else
            valp3 = ADRESH;
        PIR1bits.ADIF = 0;
    }
    if (INTCONbits.T0IF){
        cont++;
        PWM_manual(valp3, cont);
    }
}

void main(void) {
    setup();
    while(1){
        if(ADCON0bits.GO == 0){
            if(ADCON0bits.CHS == 0b0000)
                ADCON0bits.CHS = 0b0001;
            else if(ADCON0bits.CHS == 0b0001)
                 ADCON0bits.CHS = 0b0010;    
            else        
                ADCON0bits.CHS = 0b0000;
            __delay_us(1000); //1000
            ADCON0bits.GO = 1;
        }
 
    }
    return;
}

void setup(void){
    ANSEL = 0b00000111;
    ANSELH = 0;
    
    TRISA = 0b00000011;
    PORTA = 0;
    TRISD = 0;
    PORTD = 0;
    
    OSCCONbits.IRCF = 0b0111;
    OSCCONbits.SCS = 1;
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0b0000;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC1 = 1;
    CCP1CONbits.P1M = 0;
    CCP1CONbits.CCP1M = 0b1100;
    CCP2CONbits.CCP2M = 0b1100;
    CCPR1L = 0x0F;
    CCP1CONbits.DC1B = 0;
    CCPR2L = 0x0F;
    CCP2CONbits.DC2B1 = 0;
    CCP2CONbits.DC2B0 = 0;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b011;
    TMR0 = 6;
    PR2 = 255;
    T2CONbits.T2CKPS = 0b11;
    T2CONbits.TMR2ON = 1;
    PIR1bits.TMR2IF = 0;
    while (PIR1bits.TMR2IF == 0);
    PIR1bits.TMR2IF = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC1 = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    INTCONbits.TMR0IE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.T0IF = 0;
}