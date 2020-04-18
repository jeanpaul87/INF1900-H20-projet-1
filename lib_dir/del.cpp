#include "del.h"

// Constructeur par defaut
DEL::DEL() {
    quelDEL_ = GAUCHE;
};

// Constructeur avec parametres
DEL::DEL(COTE_DEL quelDEL){
    quelDEL_ = quelDEL;
}

// Destructeur par defaut
DEL::~DEL() {}

// Pour eteindre la DEL
void DEL::eteint() {
    PORTD &= ETEINT;
}

// Pour allumer la DEL en rouge
void DEL::rouge() {
    if(quelDEL_ == DROITE){
        PORTD &= 0x7B;
        PORTD |= 0x80;
    }
    
    else{ 
        PORTD &= 0XFC;
        PORTD |= ROUGE;
    }
}

// Pour allumer la DEL en vert
void DEL::vert() {
    if(quelDEL_ == DROITE){
        PORTD &= 0x7B;
        PORTD |= 0x04;
    }
    else{ 
        PORTD &= 0XFC;
        PORTD |= VERT;
    }
}

// Pour allumer la DEL en jaune
void DEL::ambre() {
    if(quelDEL_ == DROITE){
        PORTD &= 0x7B;
        PORTD |= 0x04;
        _delay_ms(5);
        PORTD &= 0x7B;
        PORTD |= 0x80;
        _delay_ms(5);
    }
    else{
        PORTD &= 0XFC;
        PORTD |= ROUGE;
        _delay_ms(5);
        PORTD &= 0XFC;
        PORTD |= VERT;
        _delay_ms(5);
    }
}

// Pour faire clignoter la DEL pendant une certaine duree
// @param duree : delai entre chaque clignotement
// @param couleur : couleur de la LED
void  DEL::allumerPendant(const uint8_t duree, COULEUR couleur) {
    if (couleur == AMBRE) {
        for(int i = 0; i < duree / 10; i++){
            ambre();
        }
    } else if (couleur == ROUGE) {
        rouge();
        for(int i = 0; i < duree; i++){
            _delay_ms(1);
        }
    } else if (couleur == VERT) {
        vert();
        for(int i = 0; i < duree; i++){
            _delay_ms(1);
        }
    } else {
        eteint();
    }
}