// Cours:       INF1900 - Projet initial de système embarqué (H20)
// Auteurs:     ALTURK, Jean-Michel
//              KHOUEIRY, Jean-Paul
//              GILBERT, Kim
//              LI, Ruijie
// Date:        17 avril 2020

#ifndef DEL_H
#define DEL_H

#define F_CPU 8000000L

#include <util/delay.h>
#include <avr/io.h>


enum COULEUR {ETEINT, VERT, ROUGE, AMBRE};
enum COTE_DEL {GAUCHE, DROITE};

class DEL {
    public:
        DEL();
        DEL(COTE_DEL quelDEL);
        ~DEL();
        void eteint();
        void rouge();
        void vert();
        void ambre();
        void allumerPendant(const uint8_t duree, COULEUR couleur);
    private:
        COTE_DEL quelDEL_;
};

#endif
