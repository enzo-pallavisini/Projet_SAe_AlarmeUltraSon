#include "Projet_AlarmeUltraSon.hpp"
  

void setup()  
{ 
  Serial.begin(9600);
  Serial.println("Init the sensor");
  Init_clavier();
  Init_Led();
  Init_Buzzer();
  //Init_Can();
  Init_interruption();
} 

void loop() 
{ 
  Lire_code(code);
  Verifier_code(code);
}  
