#pragma once
#include <arduino.h>

 // # Editor     : ZRH from DFRobot 
 // # Date       : 29.08.2014 
   
 // # Product name: URM V4.0 ultrasonic sensor 
 // # Product SKU : SEN0001 
 // # Version     : 1.0 

 // # Description: 
 // # The Sketch for scanning 180 degree area 3-500cm detecting range 
 // # The sketch for using the URM37 PWM trigger pin mode from DFRobot   
 // #   and writes the values to the serialport 
 // # Connection: 
 // #       Vcc (Arduino)    -> Pin 1 VCC (URM V4.0) 
 // #       GND (Arduino)    -> Pin 2 GND (URM V4.0) 
 // #       PIN 11 (Arduino) -> Sortie LED (PB3)
 // #       PIN 10 (Arduino) -> Sortie Buzzer (PB2)
 // #       Pin 9 (Arduino)  -> Pin 4 ECHO (URM V4.0) (PB1)
 // #       Pin 8 (Arduino)  -> Pin 6 COMP/TRIG (URM V4.0) (PB0)
 // #       Pin A0 (Arduino) -> Pin 7 DAC (URM V4.0) 
 // #       Pin 7 (Arduino)  -> Entrée bouton poussoirs    
 // # Working Mode: PWM trigger pin  mode. 



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
void Mesure_distance();
void Init_Led();
void Changer_Led();
void Clignotter_led();
void Init_Buzzer();
void Init_clavier();
char Lecture_clavier();
void Lire_code(char code[]);
void Verifier_code  (char code[]);
void Init_BP();
bool Bp_appuyee();
void Definir_code();
