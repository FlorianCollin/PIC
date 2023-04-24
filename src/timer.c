#include "timer.h"

void timer1_init(void) {
    // Set up Timer1 to use an external 32.768 kHz crystal
    T1CONbits.T1OSCEN = OSCILLATOR_ENABLED;
    T1CONbits.TMR1CS = EXTERNAL_CLOCK;
    T1CONbits.T1CKPS = T1_PRESCALER_DIV1; 
    T1CONbits.TMR1ON = ENABLE_TIMER_1; 
    

    // Configure Timer1 interrupt
    PIR1bits.TMR1IF = 0; // Clear Timer1 interrupt flag
    PIE1bits.TMR1IE = 1; // Enable Timer1 interrupt
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1; // Enable global interrupts
}

void ccp1_init(void) {
    // Configuration du module CCP1 en mode "Compare"
    CCP1CONbits.CCP1M = 0b1001; // Configure CCP1 en mode "Compare" avec interruption sur correspondance

    // Charge la valeur de comparaison
    //CCPR1 = 32768; // Demi-parcours du Timer1  <----- ici 32768 correspond à la moitié du timer soit 1s
    CCPR1 = 32768/4;
    // Active l'interruption pour le module CCP1
    PIR1bits.CCP1IF = 0; // Efface le drapeau d'interruption CCP1
    PIE1bits.CCP1IE = 1; // Active l'interruption pour le module CCP1
}