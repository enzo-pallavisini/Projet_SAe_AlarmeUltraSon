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
 //#        PIN 10 (Arduino)  -> Sortie LED
 // #       Pin 9 (Arduino)  -> Pin 4 ECHO (URM V4.0) 
 // #       Pin 8 (Arduino)  -> Pin 6 COMP/TRIG (URM V4.0) 
 // #       Pin A0 (Arduino)  -> Pin 7 DAC (URM V4.0) 
 // # Working Mode: PWM trigger pin  mode. 


#define  Measure  1     //Mode select 
#define  Mask2   (1 << 2)
#define  Mask3   (1 << 3)
#define  Mask4   (1 << 4)
#define  Mask6   (1 << 6)
 

extern char code[5];    //5 car code + caractère spécial "\0" pour la fin de la chaîne de caractères


const char clavier[4][3] = {
 {'1','2','3'},
 {'4','5','6'},
 {'7','8','9'},
 {'A','0','B'}
};


extern int Sortie_Led;
extern int URECHO;        // PWM Output 0-25000US,Every 50US represent 1cm 
extern int URTRIG;         // PWM trigger pin 
extern int sensorPin;     // select the input pin for the potentiometer 
extern int sensorValue;    // variable to store the value coming from the sensor 
extern unsigned int DistanceMeasured; 


void PWM_Mode();
void Allumer_Led();
void Init_clavier();
void Init_port();
void Lire_code(char code[]);
void Verifier_code  (char code[]);