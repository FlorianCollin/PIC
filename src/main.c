#include <xc.h>
#include "timer.h"
#include "lib_LCD.h"

//Configure les bits de configuration du microcontr?leur, comme l'oscillateur, le watchdog timer, la protection m?moire, etc.

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
typedef struct timeHMS TIME;
//proto 

char encodeChiffre(int chiffre);
int print_time(TIME time);
void time_reset(TIME *time);
int incr_time(TIME *time);



#define _XTAL_FREQ 20000000


struct timeHMS {
    int h1, h2;
    int m1, m2;
    int s1, s2;
    char *t;
};



volatile unsigned char interrupt_counter = 0;

void __interrupt() IT() // Cette fonction est appel? ? chaque fois que le programme detecte une interuption
{
    if (PIR1bits.TMR1IF) //si on a PIR1bits.TMR1IF qui vaut 1 alors le Timer1 ? d?border (il s'agit d'un flag)
    {
        
        // G?re l'interruption du Timer1
        LED_PIN = 0; // ?teint la LED
        TMR1 = 0; // R?initialise la valeur du Timer1
        PIR1bits.TMR1IF = 0; // Efface le drapeau d'interruption du Timer1 pour pouvoir d?tecter la prochaine interruption
    }

    if (PIR1bits.CCP1IF) // On verifie si le flag d'interruption du module CPP est ? 1
    {
        
        
        // G?re l'interruption du module CCP1
        LED_PIN = 1; // Allume la LED
        PIR1bits.CCP1IF = 0; // Efface le drapeau d'interruption CCP1, cela va permettre de detecter la prochaine interuprtion du module CPP1
    }
}



void init() {
    LED_TRIS = 0; // la led est set en output
    LED_PIN = 0; // Turn off LED

    timer1_init();
    ccp1_init();
    // Active les interruptions globales et p?riph?riques
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1; // Enable global interrupts

}

    


int main(void) {
    TIME time;

    init();
    lcd_init();
    lcd_home();
    
    time.h1 = 0;
    time.h2 = 0;
    time.m1 = 0;
    time.m2 = 0;
    time.s1 = 0;
    time.s2 = 0;

    // lcd_write_instr_8bits(1,0,'h');
    // lcd_write_instr_8bits(1,0,'e');
    // lcd_write_instr_8bits(1,0,'l');
    // lcd_write_instr_8bits(1,0,'l');
    // lcd_write_instr_8bits(1,0,'o');
    
    lcd_pos(3,0);
    incr_time(&time);

    print_time(time);
    
    lcd_pos(0,1);
    lcd_puts("Collin & Dufresh");



    while (1) {
        __delay_ms(70); //adaptation de manière expérimental !
        lcd_home();
        //lcd_clear();
        lcd_pos(3,0);
        print_time(time);
        incr_time(&time);        
    }   

    return 0;
}


char encodeChiffre(int chiffre) {
    char encodage[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    if (chiffre >= 0 && chiffre <= 9) {
        return encodage[chiffre];
    }
    
    return '\0'; // Retourne le caractère nul si le chiffre est invalide
}

int print_time(TIME time) {
    lcd_putch(encodeChiffre(time.h1));
    lcd_putch(encodeChiffre(time.h2));
    lcd_putch(':');
    lcd_putch(encodeChiffre(time.m1));
    lcd_putch(encodeChiffre(time.m2));
    lcd_putch(':');
    lcd_putch(encodeChiffre(time.s1));
    lcd_putch(encodeChiffre(time.s2));

}


void time_reset(TIME *time) {
    (*time).h1 = 0;
    (*time).h2 = 0;
    (*time).m1 = 0;
    (*time).m2 = 0;
    (*time).s1 = 0;
    (*time).s2 = 0;

}

int incr_time(TIME *time) {
    // Je fais le choix de manipuler la table ascii et non des entiers
    
    // OO:OO:OO
    
    (*time).s2 ++;
    if ((*time).s2 == 10) {
        (*time).s1 ++;
        (*time).s2 = 0;
    }
    if ((*time).s1 == 6) {
        (*time).m2 ++;
        (*time).s1 = 0;
    }
    if ((*time).m2 == 10) {
        (*time).m1 ++;
        (*time).m2 = 0;
    }
    if ((*time).m1 == 6) {
        (*time).h2 ++;
        (*time).m2 = 0;
    }
    if ((*time).h2 == 10) {
        (*time).h1 ++;
        (*time).h2 = 0;
    }
    if ((*time).h1 == 2 && (*time).h2 == 4 ){
        time_reset(time);
    }
    
}
   