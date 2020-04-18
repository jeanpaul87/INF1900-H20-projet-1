#include "sonar.h"

// Permet d'envoyer le signal d'echo (TRG) des trois sonars
void Sonar::envoyerSignal(){
    PORTB |=  0x01;
    _delay_us(10);
    PORTB &= 0xFE;
}

// Permet de lire les signaux (echo) de chaque sonar
// @param s1, s2, s3 represente chaque sonar passe par reference
void Sonar::lireSignal(unsigned int& s1, unsigned int& s2, unsigned int& s3){
    unsigned int sonar1 = 0;
    unsigned int sonar2 = 0;
    unsigned int sonar3 = 0;
    envoyerSignal();
    // assuming that while translates to
    // mov, add, cmp, jmp 
    const unsigned int tickWhile = 4;
    // assuming that if translates to
    // mov, cmp, jmp
    const unsigned int tickIf = 3;
    // assuming that an operation translates to
    // mov, add
    const unsigned int tickOp = 2;
    const double periodClk = 0.125;

    // wait till the signal comes
    while((PINA & 0x07) == 0x00) {}

    cli();
    while(PINA & 0x07) {

        if(PINA & 0x01){
            sonar1 += 1;
        }
        if(PINA & 0x02){     
            sonar2 += 1;
        }
        if(PINA & 0x04){ 
            sonar3 += 1;
        }
        _delay_us(10);
    }
    
    sei();
    // according to documentation, the echo line
    // raises at the same time for all
    // and lowers when the echo is received

    // sort 3 sonars from smalles to biggest
    unsigned int sonars[3] = {sonar1, sonar2, sonar3};
    unsigned int min = sonars[0];
    for(int i = 0; i < 3; i++){
        if(sonars[i] < min){
            min = sonars[i];
            int tmp = sonars[0];
            sonars[0] = min;
            sonars[i] = tmp;
        }
    }

    if(sonars[1] > sonars[2]){
        int tmp = sonars[1];
        sonars[1] = sonars[2];
        sonars[2] = tmp;
    }
    double addSonar1 = 0;
    double addSonar2 = 0;
    double addSonar3 = 0;
    double averageOp[3] = {3,2,1}; // initial values
    // shortest is 3, as it always has 3 operations
    // ex. to illustrate
    // lenght = time that pin is 1
    // shortest : ------
    //      mid : -----------------
    //  longest : --------------------------------
    
    averageOp[0] = 3;
    averageOp[1] = (double)(sonars[0] * 3 + (sonars[1] - sonars[0]) * 2) / (double)sonars[1];
    averageOp[2] = (double)(sonars[0] * 3 + (sonars[1] - sonars[0]) * 2 + (sonars[2] - sonars[1]) * 1) / (double)sonars[2];

    for(int i = 0; i < 3; i++){
        // index 0 = shortest so 3 add operations
        // index 2 = middle so 2 add operations
        // index 3 = longest so 1 add operations
        if(sonar1 == sonars[i]){ addSonar1 = averageOp[i]; }
        if(sonar2 == sonars[i]){ addSonar2 = averageOp[i]; }
        if(sonar3 == sonars[i]){ addSonar3 = averageOp[i]; }
    }
    // if executes 3x / loop
    // while executes 1x / loop
    double adjFactor1 = 0;
    double adjFactor2 = 0;
    double adjFactor3 = 0;

    adjFactor1 = 3*tickIf + tickWhile + addSonar1*tickOp;
    adjFactor2 = 3*tickIf + tickWhile + addSonar2*tickOp;
    adjFactor3 = 3*tickIf + tickWhile + addSonar3*tickOp;
    
    // Conversion de la distance en cm
    s1 = sonar1 * (10.0 + adjFactor1*periodClk) / EN_CM;
    s2 = sonar2 * (10.0 + adjFactor2*periodClk) / EN_CM;
    s3 = sonar3 * (10.0 + adjFactor3*periodClk) / EN_CM;
}

// Affiche la distance calculee par un sonar sur la LCD
// @param lcm : la LCM a utiliser
// @param distance: la distance du sonar a afficher
// @param index: index intial où commencer l'affichage sur la LCD
void Sonar::afficherDistance(LCM& lcm, unsigned int distance, int index) {
    /* char distanceStr[3];
    sprintf(distanceStr,"%d", distance); */

    char msg[5];
    
    msg[0] = (char)(48 + distance/100);
    msg[1] = '.';
    msg[2] = (char)((48 + (distance - 100*(distance/100))/10));
    msg[3] = 'm';
    msg[4] = '\0';

    lcm.write(msg, index, false); 
    

    if(distance < DISTANCE_DANGER){
        lcm[index+16] = (char*)"DNGR"; //+16 car la LCD est separee en 2 lignes de 16 positions de 0 - 31.
    }
    if(distance >= DISTANCE_DANGER && distance < DISTANCE_OK){
        lcm[index+16] = (char*)"ATTN";
    }
    if(distance >= DISTANCE_OK){
        lcm[index+16] = (char*)" OK ";
    }
}


