
/****************************************
            AFFICHEUR LCD 
          sur carte PICDEM2+
*****************************************

               Lib LCD
  
****************************************/

#ifndef __LIB_LCD
#define	__LIB_LCD

#include <xc.h>  
#include <stdint.h>  

// On introduira ici tous les symboles et macros utiles qui faciliteront 
// la maintenance et la lisibilite du fichier source correspondant





// Etape 1 : Simplification des acces
// voir https://git.io/EN111PR => 3. Developpement de la bibliotheque pour l'afficheur LCD 

typedef union {
	// définitions des bits individuellement :
	struct { 
		unsigned DB4             :1;  // bit 0
		unsigned DB5             :1;  // bit 1
		unsigned DB6             :1;  // bit 2
		unsigned DB7             :1;  // bit 3
		unsigned RS              :1;  // bit 4
		unsigned RW              :1;  // bit 5
		unsigned E               :1;  // bit 6
		unsigned POWER           :1;  // bit 7
	}; 
    
	// définition des champs :
	struct {
		unsigned DB              :4;  // bits 0 à 3 : données
		unsigned OPERATION       :2;  // bits 4 et 5 : RS et RW : définissent le type d'opération
		unsigned                 :2;  // E et POWER déjà définis, facultatif
	};
    
} LCDbits_t;

extern volatile LCDbits_t LCDbits __at(0x008); // Ca ne marche pas avec @


// Etape 2 : Developpement de fonctions d'envoi de n'importe quelle instruction
void lcd_write_instr_4bits(uint8_t rs, uint8_t rw, uint8_t data_4bits);
void lcd_write_instr_8bits(uint8_t rs, uint8_t rw, uint8_t data_8bits);
void lcd_busy();

// Etape 3 : Developpement des fonctions correspondant aux differentes instructions

// definition des patterns des commandes :
#define LCD_CLEAR_DISPLAY_INSTR   0b00000001
#define LCD_RETURN_HOME_INSTR     0b00000010
#define LCD_ENTRY_MODE_INSTR      0b00000100
#define LCD_DISPLAY_CONTROL       0b00001000
#define LCD_FONCTION_SET          0b00100000

// definition des bits specifiques aux fonctions :
// Entry mode set :
// I/D :
#define LCD_CURSOR_INC_DEC_BIT_POSN 1   // position du bit
#define LCD_CURSOR_DEC              0b0
#define LCD_CURSOR_INC              0b1
// S :
#define LCD_DISPLAY_SHIFT_BIT_POSN  0
#define LCD_DONT_SHIFT_DISPLAY      0b0
#define LCD_SHIFT_DISPLAY           0b1
//...

void lcd_clear();
void lcd_home();
void lcd_entry_mode_set(uint8_t inc_dec, uint8_t shift);
void lcd_display_control(uint8_t display, uint8_t cursor, uint8_t blink);
void lcd_cursor_display_shift(uint8_t cursor_display, uint8_t left_right);
void lcd_function_set(uint8_t data_length, uint8_t lines, uint8_t font);

// Etape 4 : Developpement de la fonction d'initialisation
void lcd_init();

// Etape 5 : Developpement des fonctions utilisateur restantes.
void lcd_putch(char c);
void lcd_puts(const char *s);
void lcd_shift_cursor(int8_t amount);
void lcd_pos(uint8_t pos, uint8_t ligne);

#endif	/* __LIB_LCD */
