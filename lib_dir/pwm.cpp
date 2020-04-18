#include "pwm.h"

// Constructeur par parametres
// Permet d'initialiser les registres en mode Phase Correct 8-bit selon la broche specifiee
// @param pin : broche a utiliser
PWM::PWM(PIN pin): pin_(pin) {
    switch (pin) {
        case D4:
            pin_ = D4;
            // Remise a zero du compteur lors d'une comparaison 
            // reussie avec OCR1B
            TCCR1A |= (1 << COM1B1);
            break;
        case D5:
            pin_ = D5;
            // idem, mais avec OCR1A
            TCCR1A |= (1 << COM1A1);
            break;
    };
    TCCR1A |= (1 << WGM10); // mode PWM Phase Correct 8 bits
    TCCR1B |= 1 << CS11;    // prescaler de 8
    TCCR1C = 0;
}

// Destructeur par defaut
PWM::~PWM() {}

// Pour reinitialiser les registres en mode normal
void PWM::normalMode() {
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCCR1C = 0x00;
}

// Permet de choisir la valeur de la periode active de la PWM ("duty cycle")
// @param pourcentage: pourcentage qui correspond a la periode active (valeur entre 0 et 99)
void PWM::ajusterPourcentage(uint8_t pourcentage) {
    
    switch (pin_) {
        case D4:
            OCR1B = (TOP_VALUE * pourcentage) / 100;
            break;
        case D5:
            OCR1A = (TOP_VALUE * pourcentage) / 100;
    }
}

// Pour initialiser les registres en mode Clear Timer on Compare (CTC)
// Methode utiliser pour la minuterie 
void PWM::startTimerCTCMode(uint8_t duration) {
    // Avec horloge divisée par 1024
    // Interruption après la durée spécifiée
    TCNT1 = 0;
    OCR1A = duration;

    // Mode CTC
    TCCR1A |= 1 << COM1A1 | 1 << WGM12;

    // Horloge divisee en 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Déjà comme ça (ne pas changer)
    TCCR1C = 0;

    // Pour activer les interruptions par le timer
    TIMSK1 |= (1 <<  OCIE1A);
}