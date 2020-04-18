// Cours:       INF1900 - Projet initial de système embarqué (H20)
// Auteurs:     ALTURK, Jean-Michel
//              KHOUEIRY, Jean-Paul
//              GILBERT, Kim
//              LI, Ruijie
// Date:        17 avril 2020
#ifndef AFFICHEUR_H
#define AFFICHEUR_H
#define F_CPU 8000000L

#include <avr/io.h>
#include "avr/interrupt.h"

enum DIGIT_VALUE
{
    ZERO = 0xFC,
    ONE = 0x60,
    TWO = 0xDA,
    THREE = 0xF2,
    FOUR = 0x66,
    FIVE = 0xB6,
    SIX = 0xBE,
    SEVEN = 0xE0,
    EIGHT = 0xFE,
    NINE = 0xF6,
    DASH = 0x02,
};

//  5 afficheurs 7 segments:
//  -------------------------------
//  | 1st | 2nd | 3rd | 4th | 5th |
//  -------------------------------
enum POSITION {
    FIRST = 0xF7,
    SECOND = 0xEF,
    THIRD = 0xDF,
    FOURTH = 0xBF,
    FIFTH = 0x7F,
};

// État de la machine à états finis
enum ETAT {
    INIT,
    S1,
    S2,
    S3,
    S4,
};

const int NB_CYCLES_AVANT_AFFICHAGE = 781; //  ~100 ms avec prescaler de 1024
const int NB_ETATS = 5;

class Afficheur
{
    public:
        Afficheur();
        ~Afficheur() = default;
        
        void afficherValeurUnique(uint8_t, POSITION);
        void afficherSur7Segments(int, int);
        
        static uint8_t convertirEn7Segments(int);
        static int extraireUniteDuNombre(unsigned int);
        static int extraireDizaineDuNombre(unsigned int);
        void initCompareMatchInterrupt(int);
        void initOverflowInterrupt();
};

#endif