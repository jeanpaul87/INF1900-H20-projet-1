#include "moteur.h"
#include "afficheur.h"

// Constructeur avec parametres
// @param roueGauche :  PWM qui controlera la roue gauche du robot
// @param roueDroite :  PWM qui controlera la roue droite du robot
Moteur::Moteur(PWM roueGauche, PWM roueDroite): roueGauche_(roueGauche), roueDroite_(roueDroite), vitesseDroite_(0), vitesseGauche_(0) {}

// Permet au robot d'avancer a vitesse constante en ligne droit ou en tournant
// @param vitesseGauche: vitesse de la roue gauche
// @param vitesseDroite: vitesse de la roue droite
void Moteur::bouger(int vitesseGauche, int vitesseDroite) {
    Afficheur afficheur;

    vitesseGauche_ = vitesseGauche;
    vitesseDroite_ = vitesseDroite;


    roueGauche_.ajusterPourcentage(vitesseGauche < 0 ? -1 * vitesseGauche : vitesseGauche);
    roueDroite_.ajusterPourcentage(vitesseDroite < 0 ? -1 * vitesseDroite : vitesseDroite);
    
    DEL droite = DEL(DROITE);
    DEL gauche = DEL(GAUCHE);

    afficheur.afficherSur7Segments(getVitesseGauche(),getVitesseDroite());

    if(vitesseGauche < 0){
        gauche.rouge();
    }
    else{
        gauche.vert();
    }
    if(vitesseDroite < 0){
        droite.rouge();
    }
    else{
        droite.vert();
    }
}

// Permet au robot d'accelerer
// @param delta_pourcenta_gauche : variation de la vitesse de la roue gauche
// @param delta_pourcentage_droite : variation de la vitesse de la roue droite
void Moteur::modifierVitesse(int delta_pourcentage_Gauche, int delta_pourcentage_Droite){
    Afficheur afficheur;

    vitesseDroite_ += delta_pourcentage_Droite;
    vitesseGauche_ += delta_pourcentage_Gauche;

    roueGauche_.ajusterPourcentage(vitesseGauche_ < 0 ? -vitesseGauche_ : vitesseGauche_);
    roueDroite_.ajusterPourcentage(vitesseDroite_ < 0 ? -vitesseDroite_ : vitesseDroite_);
    
    DEL droite = DEL(DROITE);
    DEL gauche = DEL(GAUCHE);

    afficheur.afficherSur7Segments(getVitesseGauche(),getVitesseDroite());

    if(vitesseGauche_ < 0){
        gauche.rouge();
    }
    else{
        gauche.vert();
    }
    
    if(vitesseDroite_ < 0){
        droite.rouge();
    }
    else{
        droite.vert();
    }
}

// Permet au robot d'arreter completement
void Moteur::arreter() {
    roueGauche_.ajusterPourcentage(VITESSE_NULLE);
    roueDroite_.ajusterPourcentage(VITESSE_NULLE);
}


// Getter pour la vitesse gauche
int Moteur::getVitesseGauche(){
    return vitesseGauche_;
}

// Getter pour la vitess droite
int Moteur::getVitesseDroite(){
    return vitesseDroite_;
}