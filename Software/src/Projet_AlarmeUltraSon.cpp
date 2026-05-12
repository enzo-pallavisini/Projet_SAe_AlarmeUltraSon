 #include "Projet_AlarmeUltraSon.hpp"

  
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
 #define  Mask2   (1<<2)
 #define  Mask3   (1<<3)
 #define  Mask4   (1<<4)
 #define  Mask6   (1<<6)

 char code[5];    //5 car code + caractère spécial "\0" pour la fin de la chaîne de caractères


 const char clavier[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'A','0','B'}
 };


 int Sortie_Led = 10;
 int URECHO = 9;        // PWM Output 0-25000US,Every 50US represent 1cm 
 int URTRIG = 8;         // PWM trigger pin 
 int sensorPin = A0;     // select the input pin for the potentiometer 
 int sensorValue = 0;    // variable to store the value coming from the sensor 
 unsigned int DistanceMeasured = 0; 
 
 
 void PWM_Mode()                             // a low pull on pin COMP/TRIG  triggering a sensor reading 
 {  
   Serial.print("Distance Measured="); 
   digitalWrite(URTRIG, LOW); 
   digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses   
   if(Measure) 
   { 
     unsigned long LowLevelTime = pulseIn(URECHO, LOW) ; 
     if(LowLevelTime>=45000)                 // the reading is invalid. 
     { 
       Serial.print("Invalid"); 
     } 
     else{ 
     DistanceMeasured = LowLevelTime /50;   // every 50us low level stands for 1cm 
     Serial.print(DistanceMeasured); 
     Serial.println("cm"); 
   } 
  
   } 
   else { 
     sensorValue = analogRead(sensorPin);  
     if(sensorValue<=10)                   // the reading is invalid. 
     { 
       Serial.print("Invalid"); 
     } 
     else { 
     sensorValue = sensorValue*0.718;       
     Serial.print(sensorValue); 
     Serial.println("cm"); 
     } 
   }  
 } 


 void Allumer_Led()
 {
   if(DistanceMeasured < 70)
   {
    digitalWrite(Sortie_Led,HIGH);
   }
   else
   {
    digitalWrite(Sortie_Led,LOW);
   }
 }


 void init_clavier()
 {
    PORTD &= ~((1 << PD6) | (1 << PD2) | (1 << PD3) | (1 << PD4));
 }


 char lecture_clavier()
 {
  for(int ligne = 0; ligne < 4; ligne++)
  {
    switch(ligne)
    {
        case 0: PORTD |= (1 << PD6); break;
        case 1: PORTD |= (1 << PD2); break;
        case 2: PORTD |= (1 << PD3); break;
        case 3: PORTD |= (1 << PD4); break;
    }
    if(PINC & (1 << PC3)) return clavier[ligne][0];
    if(PINC & (1 << PC4)) return clavier[ligne][1];
    if(PINC & (1 << PC5)) return clavier[ligne][2];
  }
  return 0;
 }

 
 void init_port()
 {
    DDRD |= (1 << PD6) | (1 << PD2) | (1 << PD3) | (1 << PD4);    // Config lignes = sorties
    DDRC &= ~((1 << PC3) | (1 << PC4) | (1 << PC5));     // Config colonnes = entrées
 }


 void Lire_code(char code[])
 {
  int i = 0;
  while(i < 4)
  {
    char touche = lecture_clavier();

    if(strcmp(code, "4582") == 0)     //verifie le code
    {
      code[i] = touche;
      i++;
      delay(300);
      while(lecture_clavier() != 0);    //Attendre relachement
    }  
  }  
  code[4] = '\0';
 }


 void Verifier_code(char code[])
 {
   if(strcmp(code, "4582") == 0)
   {
          //Bon code
   }
   else
   {
          //Mauvais code
   }
 }