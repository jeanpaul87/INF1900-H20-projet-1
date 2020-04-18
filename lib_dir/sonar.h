// Cours:       INF1900 - Projet initial de système embarqué (H20)
// Auteurs:     ALTURK, Jean-Michel
//              KHOUEIRY, Jean-Paul
//              GILBERT, Kim
//              LI, Ruijie
// Date:        17 avril 2020

#ifndef SONAR_H
#define SONAR_H

#define F_CPU 8000000L

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include "lcm_so1602dtr_m_fw.h"
#include "avr/interrupt.h"
#include <stdio.h>

const double EN_CM = 58.0; //il faut diviser uS par 58 pour trouver la distance en cm.
const int DISTANCE_DANGER = 100;
const int DISTANCE_OK = 300;

class Sonar {
public:
    //static char* getMessage();
	static void envoyerSignal() ;
	static void lireSignal(unsigned int& s1, unsigned int& s2, unsigned int& s3);
	static void afficherDistance(LCM& lcm, unsigned int distance, int index);
};

#endif
