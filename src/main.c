#include <xc.h>
#include "timer.h"
#include "lib_LCD.h"

//Configure les bits de configuration du microcontr�leur, comme l'oscillateur, le watchdog timer, la protection m�moire, etc.

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


//on definit des macro pour acceder plus facilement au port RBO
#define LED_PIN PORTBbits.RB0 // RBO est le pin de la led
#define LED_TRIS TRISBbits.TRISB0 // bit de direction (TRIS) pour le port RB0

volatile unsigned char interrupt_counter = 0;

void __interrupt() IT() // Cette fonction est appel� � chaque fois que le programme detecte une interuption
{
    if (PIR1bits.TMR1IF) //si on a PIR1bits.TMR1IF qui vaut 1 alors le Timer1 � d�border (il s'agit d'un flag)
    {
        // G�re l'interruption du Timer1
        LED_PIN = 0; // �teint la LED
        TMR1 = 0; // R�initialise la valeur du Timer1
        PIR1bits.TMR1IF = 0; // Efface le drapeau d'interruption du Timer1 pour pouvoir d�tecter la prochaine interruption
    }

    if (PIR1bits.CCP1IF) // On verifie si le flag d'interruption du module CPP est � 1
    {
        // G�re l'interruption du module CCP1
        LED_PIN = 1; // Allume la LED
        PIR1bits.CCP1IF = 0; // Efface le drapeau d'interruption CCP1, cela va permettre de detecter la prochaine interuprtion du module CPP1
    }
}



void init() {
    LED_TRIS = 0; // la led est set en output
    LED_PIN = 0; // Turn off LED

    timer1_init();
    ccp1_init();
    // Active les interruptions globales et p�riph�riques
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1; // Enable global interrupts

}

int main(void) {
    init();
    lcd_init();
    lcd_clear();

    while (1) {

    lcd_display_control(1,1,1);
  
    }   

    return 0;
}
