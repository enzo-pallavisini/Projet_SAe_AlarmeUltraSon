#include "Projet_AlarmeUltraSon.hpp"


void setup()  
{ 
  Serial.begin(9600);
  Init_clavier();
  Init_Led();
  Init_Buzzer();
  Init_BP();
  Init_URM37();
}

void loop() 
{ 
  switch(alarme)
  {
    case 0:
      while(Lecture_clavier() == 0)
      {
        if(PIND & (1 << Entree_BP))
        {
          Definir_code();
        }
      }
      if(Lecture_clavier() != 0)
      {
        Lire_code(code);
        Verifier_code(code);
      }
    break;  
    case 1:
      while(Lecture_clavier() == 0)
      {
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
          Frequence(0);
        }
      }
      if(Lecture_clavier() != 0)
      {
        Lire_code(code);
        Verifier_code(code);
      }
  }
}  
