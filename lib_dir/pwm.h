// Cours:       INF1900 - Projet initial de système embarqué (H20)
// Auteurs:     ALTURK, Jean-Michel
//              KHOUEIRY, Jean-Paul
//              GILBERT, Kim
//              LI, Ruijie
// Date:        17 avril 2020

#ifndef PWM_H
#define PWM_H
#define F_CPU 8000000L

#include <avr/io.h>

enum PIN {
    D4 = 0x10, // pin controlant l'oscilloscope GAUCHE
    D5 = 0x20, // pin controlant l'oscilloscope DROIT
};

const uint8_t TOP_VALUE = 255;

class PWM {
    public:
        PWM(PIN pin);
        ~PWM();
        void normalMode();
        void ajusterPourcentage(uint8_t pourcentage);
        void startTimerCTCMode(uint8_t duration);
    private:
        PIN pin_;
};

#endif