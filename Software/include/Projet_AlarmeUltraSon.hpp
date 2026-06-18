#pragma once
#include <arduino.h>

 // # Connection: 
 // #       Vcc (Arduino)    -> Pin 1 VCC (URM V4.0) 
 // #       GND (Arduino)    -> Pin 2 GND (URM V4.0) 
 // #       PIN 11 (Arduino) -> Sortie LED (PB3)
 // #       PIN 10 (Arduino) -> Sortie Buzzer (PB2)
 // #       Pin 9 (Arduino)  -> Pin 4 ECHO (URM V4.0) (PB1)
 // #       Pin 8 (Arduino)  -> Pin 6 COMP/TRIG (URM V4.0) (PB0)
 // #       Pin 7 (Arduino)  -> Entrée bouton poussoirs (PD7)
 // # Mode: PWM trigger pin mode. 


extern char code_secret[5];
extern char code[5];            //5 car code + caractère spécial "\0" pour la fin de la chaîne de caractères


const char clavier[4][3] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'A','0','B'}
};


extern int Sortie_Led;
extern int Sortie_Buzzer;
extern int Entree_BP;
extern int URECHO;
extern int URTRIG;
extern unsigned int Distance_Mesure; 
extern bool alarme;


void Frequence(int x);
void Init_URM37();
void Mesure_distance();
void Init_Led();
void Changer_Led();
void Clignoter_Led();
void Init_Buzzer();
void Init_clavier();
char Lecture_clavier();
void Lire_code(char code[]);
void Verifier_code  (char code[]);
void Init_BP();
bool Bp_appuyee();
void Definir_code();