// Cours:       INF1900 - Projet initial de système embarqué (H20)
// Auteurs:     ALTURK, Jean-Michel
//              KHOUEIRY, Jean-Paul
//              GILBERT, Kim
//              LI, Ruijie
// Date:        17 avril 2020
#define F_CPU 8000000L

#include <avr/io.h>
#include "pwm.h"
#include "moteur.h"
#include "afficheur.h"
#include "sonar.h"
#include "InterruptButton.h"
#include "del.h"
#include <string.h>


enum MANOEUVRE
{
    DETECTION,
    M1,
    M2,
    M3,
    M4,
    M5,
    M6,
    UNDEFINED
};
volatile int backupTIMSK1;
volatile int backupTIMSK2;
volatile int backupTCCR1A;
volatile int backupTCCR2A;
unsigned int sonar1, sonar2, sonar3;
MANOEUVRE manoeuvre = DETECTION;

// Permet de déterminer le message d'avertissement selon la distance calculee par un sonar
// return un tableau de trois caracteres correspondant chacun a un message (O: OK, A:ATTN, D: DNGR)
char *getMessage()
{
    char *check = (char *)malloc(4 * sizeof(char));

    unsigned int sonars[3] = {sonar1, sonar2, sonar3};
    for (int i = 0; i < 3; i++)
    {
        if (sonars[i] >= DISTANCE_OK)
        {
            check[i] = 'O';
        }
        else if (sonars[i] >= DISTANCE_DANGER && sonars[i] < DISTANCE_OK)
        {
            check[i] = 'A';
        }
        else
        {
            check[i] = 'D';
        }
    }
    return check;
}

// Permet de determniner la manoeuvre du robot a executer
// return un enum correspondant a la manoeuvre
MANOEUVRE getManoeuvre()
{
    char *msg = getMessage();
    if (strcmp(msg, "OOA") == 0)
    {
        free(msg);
        return M1;
    }
    else if (strcmp(msg, "AOO") == 0)
    {
        free(msg);
        return M2;
    }
    else if (strcmp(msg, "ODD") == 0)
    {
        free(msg);
        return M3;
    }
    else if (strcmp(msg, "DDO") == 0)
    {
        free(msg);
        return M4;
    }
    else if (strcmp(msg, "DDD") == 0)
    {
        free(msg);
        return M5;
    }
    else if (strcmp(msg, "AOA") == 0)
    {
        free(msg);
        return M6;
    }
    else
    {
        free(msg);
        return UNDEFINED;
    }
}

// Permet d'envoyer et lire le signal par les sonars
void updateSonar()
{
    Sonar::envoyerSignal();
    Sonar::lireSignal(sonar1, sonar2, sonar3);
}

// Permet l'affichage sur la LCD des distances calculees par les sonars
// @param lcd : reference vers l'afficheur LCD a utiliser
void detection(LCM &lcd)
{
    //lcd.clear();
    Sonar::afficherDistance(lcd, sonar1, 0);
    Sonar::afficherDistance(lcd, sonar2, 6);
    Sonar::afficherDistance(lcd, sonar3, 12);
}

// Permet au robot d'executer la manoeuvre 1
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void manoeuvre1(Moteur &moteur, LCM &lcd)
{
    lcd.write((char *)"Manoeuvre 1", 0, true);
    int v1 = 90;
    int v2 = 90;
    moteur.bouger(v1, v2);
    for (size_t i = 0; i < 38; i++)
    {
        moteur.modifierVitesse(-1, 0);
        _delay_ms(100);
    }
    _delay_ms(1000);

    for (size_t i = 0; i < 38; i++)
    {
        moteur.modifierVitesse(1, 0);
        _delay_ms(100);
    }
    _delay_ms(2000);
}

// Permet au robot d'executer la manoeuvre 2
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void manoeuvre2(Moteur &moteur, LCM &lcd)
{
    lcd.write((char *)"Manoeuvre 2", 0, true);
    moteur.bouger(90, 90);
    for (uint8_t i = 0; i < 38; i++)
    {
        moteur.modifierVitesse(0, -1);
        _delay_ms(100);
    }
    _delay_ms(1000);
    for (uint8_t i = 0; i < 38; i++)
    {
        moteur.modifierVitesse(0, 1);
        _delay_ms(100);
    }
    _delay_ms(2000);
}

// Permet au robot d'executer la manoeuvre 3
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void manoeuvre3(Moteur &moteur, LCM &lcd)
{
    lcd.write((char *)"Manoeuvre 3", 0, true);
    moteur.bouger(-50, 50);
    _delay_ms(1000);
    moteur.bouger(66, 66);
    _delay_ms(2000);
    moteur.bouger(50, -50);
    _delay_ms(1000);
    moteur.bouger(78, 78);
    _delay_ms(2000);
}

// Permet au robot d'executer la manoeuvre 4
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void manoeuvre4(Moteur &moteur, LCM &lcd)
{
    lcd.write((char *)"Manoeuvre 4", 0, true);
    moteur.bouger(50, -50);
    _delay_ms(1000);
    moteur.bouger(66, 66);
    _delay_ms(2000);
    moteur.bouger(-50, 50);
    _delay_ms(1000);
    moteur.bouger(78, 78);
    _delay_ms(2000);
}

// Permet au robot d'executer la manoeuvre 5
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void manoeuvre5(Moteur &moteur, LCM &lcd, Afficheur& afficheur)
{
    lcd.write((char *)"Manoeuvre 5", 0, true);
    moteur.bouger(50, -50);
    _delay_ms(2000);
    moteur.bouger(0, 0);
    for (uint8_t i = 0; i < 21; i++)
    {
        moteur.modifierVitesse(3, 3);
        _delay_ms(125);
    }
    _delay_ms(2000);
}

// Permet au robot d'executer la manoeuvre 6
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void manoeuvre6(Moteur &moteur, LCM &lcd, Afficheur& afficheur){
    lcd.write((char *)"Manoeuvre 6", 0, true);
    int vitesseInitDroite = 90, vitesseInitGauche = 90;
    moteur.bouger(vitesseInitDroite, vitesseInitGauche);
    for (uint8_t i = 0; i < 7; i++)
    {
        moteur.modifierVitesse(-7, -7);
        vitesseInitDroite -= 7;
        vitesseInitGauche -= 7;
        _delay_ms(500);
    }
}

// Permet d'afficher un message d'avertissement lorsque la manoeuvre n'est pas reconnu
// @param moteur : reference vers le moteur du robot
// @param lcd : reference vers l'afficheur LCD a utiliser
void undef(Moteur &moteur, LCM &lcd)
{
    lcd.write("Combinaison non evaluee", 0, true);
    _delay_ms(2000);
}

void init()
{
    cli();
    DDRD = 0xF7;
    DDRC = 0xFF;
    DDRB = 0xFE;
    DDRA = 0xF8; // 0B1111 1000
    sei();
}
ISR(INT1_vect){
    cli();
    manoeuvre = getManoeuvre();
    TIMSK1 = backupTIMSK1;
    TIMSK2 = backupTIMSK2;
    TCCR1A = backupTCCR1A;
    TCCR2A = backupTCCR2A;
    sei();
}
int main()
{
    cli();
    // Configuration des ports
    init();

    // Instances des objets nécessaire au programme
    Afficheur afficheur;
    afficheur.initOverflowInterrupt();

    LCM lcd(&DDRB, &PORTB);
    PWM gauche(D4);
    PWM droite(D5);
    Moteur moteur(gauche, droite);

    InterruptButton button = InterruptButton();
    button.initInterruptButton1(FallingEdge);
    backupTIMSK1 = TIMSK1;
    backupTIMSK2 = TIMSK2;
    backupTCCR1A = TCCR1A;
    backupTCCR2A = TCCR2A;
    sei();
    for (;;){

        switch (manoeuvre){

            case DETECTION:{
                TIMSK1 = 0;
                TIMSK2 = 0;
                updateSonar();
                detection(lcd);
                PORTD = 0;
                PORTC = 0;
                PORTA &= 0X07;
                TCCR1A = 0;
                TCCR2A = 0;
                _delay_ms(250);

                /*if ((PIND & 0b00001000) == 0)
                {
                }*/
                break;
            }
            case M1:{
                manoeuvre1(moteur, lcd);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
            case M2:{
                manoeuvre2(moteur, lcd);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
            case M3:{
                manoeuvre3(moteur, lcd);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
            case M4:{
                manoeuvre4(moteur, lcd);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
            case M5:{
                manoeuvre5(moteur, lcd, afficheur);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
            case M6:{
                manoeuvre6(moteur, lcd, afficheur);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
            case UNDEFINED:{
                undef(moteur, lcd);
                lcd.clear();
                manoeuvre = DETECTION;
                break;
            }
        }
    }
    // Retourner en mode detection
    PORTD = 1 << PD3;
}