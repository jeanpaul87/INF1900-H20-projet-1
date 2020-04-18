// Cours:       INF1900 - Projet initial de système embarqué (H20)
// Auteurs:     ALTURK, Jean-Michel
//              KHOUEIRY, Jean-Paul
//              GILBERT, Kim
//              LI, Ruijie
// Date:        17 avril 2020

#ifndef MOTEUR_H
#define MOTEUR_H
#define F_CPU 8000000L

#include <avr/io.h>
#include "pwm.h"
#include "del.h"

const uint8_t VITESSE_NULLE = 0;

class Moteur{
    public:
        Moteur(PWM roueGauche, PWM roueDroite);
        ~Moteur() = default;

        void tourner(bool versLaDroite, bool versLaGauche, uint8_t vitesseGauche, uint8_t vitesseDroite);
        void modifierVitesse(int delta_pourcentage_Gauche, int delta_pourcentage_Droite);
        void arreter();
        void bouger(int vitesseGauche, int vitesseDroite);
        int getVitesseDroite();
        int getVitesseGauche();
    private:
        PWM roueGauche_;
        PWM roueDroite_;
        int vitesseDroite_;
        int vitesseGauche_;
};

#endif
