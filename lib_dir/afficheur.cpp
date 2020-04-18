#include "afficheur.h"

volatile uint8_t etat = 0; // (declarer ici, car ne fonctionne pas dans le .h)
volatile uint8_t positions[5] = {FIRST,SECOND,THIRD,FOURTH,FIFTH};
volatile uint8_t tableauNombre[5];

// Constructeur par defaut
Afficheur::Afficheur() {}

// Permet d'afficher une valeur sur l'un des 5 afficheurs
// @param digit: valeur numerique a afficher
// @param position: position de l'afficheur a activer
void Afficheur::afficherValeurUnique(uint8_t digit, POSITION position) {
    PORTA = position;
    PORTC = digit;
}

// Permet d'afficher deux nombres sous la forme |XY-UV|
// @param nombre1: le nombre qui sera afficher sur les deux afficheurs de gauche
// @param nombre2: le nombre qui sera afficher sur les deux afficheurs de droite
void Afficheur::afficherSur7Segments(int nombre1, int nombre2) {

    unsigned int n1 = nombre1 < 0 ? nombre1 * -1 : nombre1;
    unsigned int n2 = nombre2 < 0 ? nombre2 * -1 : nombre2;
    
    tableauNombre[0] = convertirEn7Segments(extraireDizaineDuNombre(n1));
    tableauNombre[1] = convertirEn7Segments(extraireUniteDuNombre(n1));
    tableauNombre[2] = DASH;
    tableauNombre[3] = convertirEn7Segments(extraireDizaineDuNombre(n2));
    tableauNombre[4] = convertirEn7Segments(extraireUniteDuNombre(n2));

}

// Permet de convertir un chiffre en 7 segments
// @param nombre: le chiffre a convertir
// @return la valeur hexadecimal du chiffre pour le 7 segment
uint8_t Afficheur::convertirEn7Segments(int chiffre)
{
    uint8_t chiffreConverti = 0;
    
    switch (chiffre) {
        case 0:
            chiffreConverti = 0xFC;
            break;
        case 1:
            chiffreConverti = 0x60;
            break;
        case 2:
            chiffreConverti = 0xDA;
            break;
        case 3:
            chiffreConverti = 0xF2;
            break;
        case 4:
            chiffreConverti = 0x66;
            break;
        case 5:
            chiffreConverti = 0xB6;
            break;
        case 6:
            chiffreConverti = 0xBE;
            break;
        case 7:
            chiffreConverti = 0xE0;
            break;
        case 8:
            chiffreConverti = 0xFE;
            break;
        case 9:
            chiffreConverti = 0xF6;
            break;
    }
    return chiffreConverti;
}

// Initialise les registres pour les interruptions avec Overflow
void Afficheur::initOverflowInterrupt() {
    cli();

    TCCR2B |= (1 << CS22) | (0 << CS21) | (0 << CS20);

    TIMSK2 |= 1 << TOIE2;

    sei();
}

// Routine d'interruption pour le Overflow
ISR (TIMER2_OVF_vect) {

    // Activation de l'afficheur et du chiffre
    PORTA = positions[etat];
    PORTC = tableauNombre[etat];

    // Changement de la position
    etat++;

    // Reinitilisation de l'etat a 0
    if (etat >= NB_ETATS) {
        etat = INIT;
    }

    TIFR2 = 0 << TOV2;
}

// Permet d'extraire l'unite d'un nombre
// @param nombre: le nombre dont on veut extraire l'unite
// @return valeur de l'unite
int Afficheur::extraireUniteDuNombre(unsigned int nombre)
{
    int unite = 0;

    if (nombre >= 0 && nombre <= 99) {
        unite = nombre % 10;
    }

    return unite;
}

// Permet d'extraire la dizainr d'un nombre
// @param nombre: le nombre dont on veut extraire la dizainr
// @return valeur de le dizaine
int Afficheur::extraireDizaineDuNombre(unsigned int nombre)
{
    int dizaine = 0;

    if (nombre >= 0 && nombre <= 99) {
        dizaine = nombre / 10;
    }

    return dizaine;
}

