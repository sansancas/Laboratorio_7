/*
 * File:   pwm_config.c
 * Author: Santiago
 *
 * Created on April 10, 2023, 10:10 PM
 */


#include <xc.h>
#include "pwm_config.h"

int main(void) {
#include <xc.h>
#include "pwm_config.h"
// Define algunos periodos de configuración del PWM
#define PWM_PERIOD_2_MS 2000
#define PWM_PERIOD_5_MS 5000
#define PWM_PERIOD_10_MS 10000

// Configura el canal especificado del PWM con el período dado en milisegundos
#define CCP1CON_REG     CCP1CON
#define CCPR1L_REG      CCPR1L
#define CCPR1H_REG      CCPR1H
#define T2CON_REG       T2CON
#define PR2_REG         PR2

// Periodos de configuración predefinidos
#define PWM_PERIOD_1KHZ 4
#define PWM_PERIOD_5KHZ 0xF
#define PWM_PERIOD_10KHZ 7

// Configura el PWM para el canal y periodo especificados
void PWM_config(char canal, float periodo_ms) {
    // Selección del registro CCPxCON según el canal
    volatile unsigned char *ccpcon_reg;
    switch (canal) {
        case 1: ccpcon_reg = &CCP1CON_REG; break;
        default: return; // canal no válido
    }
    // Configuración del registro CCPxCON
    *ccpcon_reg = 0b00001100; // modo PWM
    // Configuración del temporizador T2
    T2CON_REG = 0b00000111; // prescaler 1:16, encendido
    // Cálculo del valor de PR2 según el periodo deseado
    float periodo_us = periodo_ms * 1000;
    unsigned char pr2_value = (unsigned char)(periodo_us / (4 * 16));
    PR2_REG = pr2_value;
}

// Configura el ciclo de trabajo del PWM para el canal y duty cycle especificados
void PWM_duty(char canal, float duty) {
    // Selección del registro CCPRxL y CCPxCON según el canal
    volatile unsigned char *ccprl_reg;
    volatile unsigned char *ccpcon_reg;
    switch (canal) {
        case 1: ccprl_reg = &CCPR1L_REG;
        ccpcon_reg = &CCP1CON_REG;
        break;
        default: return; // canal no válido
    }
    // Cálculo del valor de CCPRxL según el ciclo de trabajo deseado
    unsigned int ccprl_value = (unsigned int)(duty * (PR2_REG + 1));
    *ccprl_reg = ccprl_value & 0xFF; // parte baja del valor
    // Configuración del bit DCx del registro CCPxCON
    *ccpcon_reg = (*ccpcon_reg & 0xCF) | ((ccprl_value >> 8) & 0x30);
}
void PWM_manual(uint8_t valp3, uint8_t cont){
    //cont++;
        if (cont < valp3){
            PORTDbits.RD0 = 1;
        }
        else
            PORTDbits.RD0 = 0;
        TMR0 = 6;
        INTCONbits.T0IF = 0;
    }
    
}

