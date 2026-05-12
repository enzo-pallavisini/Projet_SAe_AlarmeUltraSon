#include "Projet_AlarmeUltraSon.hpp"
  
 void setup()  
 { 
   //Serial initialization 
   Serial.begin(9600);                        // Sets the baud rate to 9600 
   pinMode(Sortie_Led,OUTPUT);
   pinMode(URTRIG,OUTPUT);                    // A low pull on pin COMP/TRIG 
   digitalWrite(URTRIG,HIGH);                 // Set to HIGH  
   pinMode(URECHO, INPUT);                    // Sending Enable PWM mode comand 
   delay(500); 
   Serial.println("Init the sensor"); 
  
  } 

 void loop() 
 { 
   PWM_Mode(); 
   delay(100);
   Allumer_Led();
   
   /*tone(10,1000);
   delay(500);
   tone(10,10000);
   delay(500);
   */
 }  
  