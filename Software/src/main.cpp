#include "Projet_AlarmeUltraSon.hpp"
  
void setup()  
{ 
  //Serial initialization 
  Serial.begin(9600);                        // Sets the baud rate to 9600 
  Serial.println("Init the sensor");
  Init_clavier();
  Init_Led();
} 

void loop() 
{ 
  Lire_code(code);
  Verifier_code(code);
}  
