#include "Projet_AlarmeUltraSon.hpp"

int Sortie_Led = 3;
int Sortie_Buzzer = 2;
int Entree_BP = 7; 
int URECHO = 9;       // PWM Output 0-25000US, 50µs represente 1cm
int URTRIG = 8;       // PWM trigger pin
unsigned int Distance_Mesure = 0;
bool alarme = 0;      // Bool à 0 pour alarme désactiver, à 1 pour alarme active
char code_secret[5] = "4582";   //5 car code + caractère spécial "\0" pour la fin de la chaîne de caractères
char code[5];         


void Init_URM37()
{
  DDRB |= (1 << URTRIG);
}


void Mesure_distance()
{
  PORTB |= (1 << URTRIG);
  Serial.print("Distance Mesure = ");
  {
    unsigned long LowLevelTime = pulseIn(URECHO, LOW);  // recupère le temps à l'état bas (en microseconde)
    if (LowLevelTime >= 45000) // lecture invalide
    {
      Serial.println("Invalid");
    }
    else
    {
      Distance_Mesure = LowLevelTime / 50; // chaque niveau bas de 50µs correspond à 1cm
      Serial.print(Distance_Mesure);
      Serial.println("cm");
    }
  }
  PORTB &= ~(1 << URTRIG);
}


void Frequence(int x)
{
  if(x == 0)
  {
    DDRB &= ~(1 << Sortie_Buzzer);  // Arret du signal
    return;
  }
  DDRB |= (1 << Sortie_Buzzer);
  ICR1 = ((16000000UL) / (8UL * 2 * x)) - 1;
  OCR1B = ICR1 / 2;   // Rapport cyclique de 50%
}


void Init_Buzzer()
{
  DDRB |= (1 << Sortie_Buzzer);
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Mode Fast PWM et Prescaler à 8
  TCCR1A |= (1 << COM1B0);  // Toggle
  ICR1 = 0;
  OCR1B = 0;
}


void Init_Led()
{
  DDRB |= (1 << Sortie_Led);
}


void Changer_Led()
{
  PORTB ^= (1 << Sortie_Led);
}


void Clignoter_Led()
{
  for(int i = 0; i < 4; i++)
  {
    Changer_Led();
    delay(250);
  }
}


void Init_clavier()
{
  DDRD |= (1 << PD6) | (1 << PD2) | (1 << PD3) | (1 << PD4); // Config lignes = sorties
  DDRC &= ~((1 << PC3) | (1 << PC4) | (1 << PC5));           // Config colonnes = entrées
}


char Lecture_clavier()
{
  for (int ligne = 0; ligne < 4; ligne++)
  {
    PORTD &= ~((1 << PD6) | (1 << PD2) | (1 << PD3) | (1 << PD4)); // Mettre lignes à 0
    switch (ligne)
    {
    case 0:
      PORTD |= (1 << PD6);
      break;
    case 1:
      PORTD |= (1 << PD2);
      break;
    case 2:
      PORTD |= (1 << PD3);
      break;
    case 3:
      PORTD |= (1 << PD4);
      break;
    }

    delay(50);  // Anti-rebond

    if (PINC & (1 << PC3))
      return clavier[ligne][0];
    if (PINC & (1 << PC4))
      return clavier[ligne][1];
    if (PINC & (1 << PC5))
      return clavier[ligne][2];
  }
  return 0;
}


void Lire_code(char code[])
{
  int i = 0;
  while (i < 4)
  {
    char touche = Lecture_clavier();
    Serial.println(touche);

    if (touche != 0) // verifie le code
    {
      code[i] = touche;
      i++;
      while (Lecture_clavier() != 0); // Attendre relachement
    }
  }
  code[4] = '\0'; // Fin de chaine
}


void Verifier_code(char code[])
{
  if (strcmp(code, code_secret) == 0) // Bon code
  {
    Changer_Led();
    Serial.println("CODE BON !");
    alarme ^= 1;
  }
  else // Mauvais code
  {
    Serial.println("CODE FAUX !");
  }
}


void Init_BP(void)
{
  DDRD &= ~(1 << Entree_BP);  // PD7 en entrée
  PORTD &= ~(1 << Entree_BP); // pull-up interne désactivé
  // PIND permet d'avoir l'info si BP est appuyé
}


bool Bp_appuyee(void)
{
  if((PIND & (1 << Entree_BP)))
  {
    return true;
  }
  else
  {
    return false;
  }
}


void Definir_code(void)
{
  while(PIND & (1 << Entree_BP))
  {
    int i = 0;
    Serial.println("Modification Code :");
    while((i < 4) & Bp_appuyee())
    {
      char touche = Lecture_clavier();
      if(touche != 0)
      {
        code_secret[i] = touche;
        while (Lecture_clavier() != 0); // Attendre relachement
        Serial.println(code_secret[i]);
        i++;
      }
    }
    code_secret[4] = '\0'; // Fin de chaine
    if(i == 4)
    {
      Serial.println("Modification réussie !");
      Clignoter_Led();
    }
    else
    {
      Serial.println("Modification annulée !");
    }
  }
}