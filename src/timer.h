
/****************************************
            AFFICHEUR LCD 
          sur carte PICDEM2+
*****************************************

              Lib Timer
  
****************************************/

#ifndef __HORLOGE
#define	__HORLOGE

#include <xc.h>  
#include <stdint.h>  

#define T1_PRESCALER_DIV8 0b11
#define T1_PRESCALER_DIV4 0b10
#define T1_PRESCALER_DIV2 0b01
#define T1_PRESCALER_DIV1 0b00

#define OSCILLATOR_ENABLED 0b1
#define OSCILLATOR_SHUT_OF 0b0

#define NOT_SYNCHRONIZE_EXTERNAL_CLOCK_INPUT 0b1
#define SYNCHRONIZE_EXTERNAL_CLOCK_INPUT 0b0

#define EXTERNAL_CLOCK 0b1
#define INTERNAL_CLOCK 0b0

#define ENABLE_TIMER_1 0b1
#define STOP_TIMER_1 0b0

void timer1_init(void);
void ccp1_init(void);


#endif 
