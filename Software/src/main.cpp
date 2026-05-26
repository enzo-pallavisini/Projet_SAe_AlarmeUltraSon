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
  if(alarme == 1)
  {
    if(Lecture_clavier() != 0)
    {
      Lire_code(code);
      Verifier_code(code);
    }
    Mesure_distance();
    if(Distance_Mesure < 50)
    {
      for(int i = 100; i < 10000; i += 20)
      {
        Frequence(i);
        _delay_ms(5);
        
      }
      for(int i = 10000; i > 100; i -= 20)
      {
        Frequence(i);
        _delay_ms(5);
      }
    }
    Frequence(0);
  }
  if((alarme == 0) & (PIND & (1 << Entree_BP)))
  {
    Definir_code();
  }
}  
