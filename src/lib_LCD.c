
/****************************************
            AFFICHEUR LCD 
          sur carte PICDEM2+
*****************************************

               Lib LCD
  
****************************************/

#include "lib_LCD.h"



#define _XTAL_FREQ 20000000

/************************************************************************
                                ETAPE 1
 Simplification des acces 
************************************************************************/

// voir dans "Header Files" => lib_LCD.h



/************************************************************************
                                ETAPE 2
 Developpement de fonctions d'envoi de n'importe quelle instruction
************************************************************************/

/// \brief Envoi d'une instruction de 4 bits vers le module LCD 
///        (utile pour l'initialisation) 
/// \param rs selectionne le registre de destination (1 bit)
/// \param rw selectionne s'il s'agit d'une lecture ou d'une ecriture (1 bit)
/// \param data_4bits est la donnee a ecrire (4 bits)


void lcd_write_instr_4bits(uint8_t rs, uint8_t rw, uint8_t data_4bits) {
/// on set les bits RS et RW
    //on affectent les bits
    //LCDbits.EP = 0b01;
    
    LCDbits.E = 0;    

    LCDbits.RS = rs;
    LCDbits.RW = rw;
    LCDbits.DB = data_4bits;


    LCDbits.E = 1;
    __delay_ms(1);
    LCDbits.E = 0;    
    __delay_ms(2);
    
    

}

/// \brief Envoi d'une instruction de 8 bits vers le module LCD
/// \param rs selectionne le registre de destination (1 bit)
/// \param rw selectionne s'il s'agit d'une lecture ou d'une ecriture (1 bit)
/// \param data_8bits est la donnee a ecrire (8 bits)

void lcd_write_instr_8bits(uint8_t rs, uint8_t rw, uint8_t data_8bits) {
    uint8_t tmp = data_8bits;
    uint8_t data_low = tmp & 0b00001111;
    uint8_t data_high = (data_8bits >> 4) & 0b00001111;

    lcd_write_instr_4bits(rs, rw, data_high);
    lcd_busy();
    lcd_write_instr_4bits(rs, rw, data_low);


}

/// \brief Attente le temps que la commande precedente ait ete traitee
void lcd_busy() {
    __delay_ms(5);
}


/************************************************************************
                                ETAPE 3
 Developpement des fonctions correspondant aux differentes instructions
************************************************************************/

// voir DS_Afficheurs_Sunplus.pdf p5, p6 et p7
// chaque fonction correspond a une instruction de l'ecran LCD

/// \brief Effacement de l'ecran
void lcd_clear() {
    lcd_write_instr_8bits(0, 0, LCD_CLEAR_DISPLAY_INSTR);
}

/// \brief Retour du curseur a l'origine
void lcd_home() {
    //return home
    lcd_write_instr_8bits(0, 0, LCD_RETURN_HOME_INSTR);
    
}

/// \brief Direction du curseur et decalage pendant l'ecriture
/// \param inc_dec (1 bit)
/// \param shift (1 bit)
void lcd_entry_mode_set(uint8_t inc_dec, uint8_t shift) {
    uint8_t mot = LCD_ENTRY_MODE_INSTR + inc_dec * 0x02 + shift * 0x01;
    lcd_write_instr_8bits(0, 0, mot);

}
/// \brief Ecran ON/OFF, Curseur ON/OFF, Clignotement ON/OFF
/// \param display (1 bit)
/// \param cursor (1 bit)
/// \param blink (1 bit)
void lcd_display_control(uint8_t display, uint8_t cursor, uint8_t blink) {
    uint8_t mot = LCD_DISPLAY_CONTROL + display * 0x04 + cursor * 0x02 + blink;
    lcd_write_instr_8bits(0, 0, mot);
}

/// \brief Deplacement du curseur ou de l'affichage d'un cran a gauche ou a droite
/// \param cursor_display (1 bit)
/// \param left_right (1 bit)
void lcd_cursor_display_shift(uint8_t cursor_display, uint8_t left_right) {
    lcd_write_instr_4bits(0b0, 0b0, 0b0001);
    
    lcd_write_instr_4bits(0b0, 0b0, 0b0000 + cursor_display << 3 + left_right << 2);

}

/// \brief Taille des donnees (4 / 8 bits), nombre de lignes, taille des caracteres
/// \param data_length (1 bit)
/// \param lines (1 bit)
/// \param font (1 bit)
void lcd_function_set(uint8_t data_length, uint8_t lines, uint8_t font) {
    uint8_t mot = LCD_DISPLAY_CONTROL + data_length * 0x10 + lines * 0x08 + font * 0x04;
    lcd_write_instr_8bits(0, 0, mot);

}





/************************************************************************
                                ETAPE 4
 Developpement de la fonction d'initialisation
************************************************************************/

// voir DS_Afficheurs_Sunplus.pdf p10 et p11

/// \brief Initialisation generale de l'afficheur
void lcd_init() {
    
    TRISD = 0x00;
    PORTDbits.RD7 = 1;
    
    __delay_ms(50);
    
    lcd_write_instr_4bits(0,0,0b0011);
    __delay_ms(5);

    lcd_write_instr_4bits(0,0,0b0011);
    __delay_us(200);

    lcd_write_instr_4bits(0,0,0b0011);

    lcd_write_instr_4bits(0,0,0b0010);

    // on peut desormais faire des instructions 8bits
    
    lcd_function_set(0,1,0); // 2 lignes 5*8 cara
    lcd_display_control(0,0,0);
    lcd_clear();
    lcd_entry_mode_set(1,0);
    lcd_display_control(1,1,1);
   
    
    

}


/************************************************************************
                                ETAPE 5
 Developpement des fonctions utilisateur restantes.
************************************************************************/

/// \brief Ecriture d'un caractere sur l'afficheur
/// \param c est le caratere a envoyer
void lcd_putch(uint8_t c) {
    lcd_write_instr_8bits(1,0,c);
}

/// \brief Ecriture d'une chaine de caracteres sur l'afficheur
/// \param s est la chaine de carateres a envoyer
void lcd_puts(const char *s) {
    char k = 0;
    char tmp = s[k];
    while (tmp != '\0') {
        lcd_putch(tmp);
        k++;

        tmp = s[k];
    }
}

/// \brief Deplacement du curseur de "amount" crans. 
/// \param amount est l'offset a appliquer au curseur
///        amount negatif = vers la gauche
///        amount positif = vers la droite
void lcd_shift_cursor(int8_t amount) {
    int k = 0;
    for (k = 0; k<amount; k++) {
        lcd_putch(' ');
    }
}

/// \brief Positionnement du curseur en (x,y). Origine en (1,1)
/// \param pos est la position horizontale
/// \param ligne est la position verticale
void lcd_pos(uint8_t pos, uint8_t ligne) {
    if (ligne == 1){
        // on ce place sur la ligne 1     ATTENTION les lignes sont num�rot�s de 0 � 1
        lcd_write_instr_8bits(0,0,0b11000000);
    }
    lcd_shift_cursor(pos);
}
