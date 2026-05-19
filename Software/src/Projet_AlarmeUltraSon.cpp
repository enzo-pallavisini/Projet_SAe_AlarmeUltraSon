#include "Projet_AlarmeUltraSon.hpp"

int Sortie_Led = 3;
int Sortie_Buzzer = 2;
int URECHO = 9;      // PWM Output 0-25000US,Every 50US represent 1cm
int URTRIG = 8;      // PWM trigger pin
int sensorPin = A0;  // select the input pin for the potentiometer
int sensorValue = 0; // variable to store the value coming from the sensor
unsigned int Distance_Mesure = 0;

char code[5];

void frequence(int x)
{
  OCR1A = ((16000000) / (2 * 8 * x)) - 1;
}


void Mesure_distance() // a low pull on pin COMP/TRIG  triggering a sensor reading
{
  Serial.print("Distance Mesure =");
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH); // reading Pin PWM will output pulses
  if (Measure)
  {
    unsigned long LowLevelTime = pulseIn(URECHO, LOW);
    if (LowLevelTime >= 45000) // the reading is invalid.
    {
      Serial.print("Invalid");
    }
    else
    {
      Distance_Mesure = LowLevelTime / 50; // every 50us low level stands for 1cm
      Serial.print(Distance_Mesure);
      Serial.println("cm");
    }
  }
  else
  {
    sensorValue = analogRead(sensorPin);
    if (sensorValue <= 10) // the reading is invalid.
    {
      Serial.print("Invalid");
    }
    else
    {
      sensorValue = sensorValue * 0.718;
      Serial.print(sensorValue);
      Serial.println("cm");
    }
  }
}

void Init_Buzzer()
{
  DDRB |= (1 << Sortie_Buzzer);
  TCCR1B |= (1 << WGM12) | (1 << CS11); // Prescaler à 8
  TCCR1A |= (1 << COM1A0);
  OCR1A = 0;
}

void Init_Led()
{
  DDRB |= (1 << Sortie_Led);
}

void Changer_Led()
{
  PORTB ^= (1 << Sortie_Led);
}

void Init_clavier()
{
  DDRD |= (1 << PD6) | (1 << PD2) | (1 << PD3) | (1 << PD4); // Config lignes = sorties
  DDRC &= ~((1 << PC3) | (1 << PC4) | (1 << PC5));           // Config colonnes = entrées
  PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);             // Pull-up colonnes
}

char lecture_clavier()
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

    delay(100);

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
    char touche = lecture_clavier();
    Serial.println(touche);

    if (touche != 0) // verifie le code
    {
      code[i] = touche;
      i++;
      while (lecture_clavier() != 0)
        ; // Attendre relachement
    }
  }
  code[4] = '\0'; // Fin de chaine
}

void Verifier_code(char code[])
{
  if (strcmp(code, "4582") == 0) // Bon code
  {
    Changer_Led();
    Serial.println("Code bon");
    Mesure_distance();
    for (int i = 9999; i > 99; i -= 20)
    {
      // frequence(i);
      //OCR1A = i;
      OCR1A = 100;
      delay(5);
      Serial.println(OCR1A);
    }
    for (int i = 99; i < 9999; i += 20)
    {
      // frequence(i);
      //OCR1A = i;
      OCR1A = 100;
      delay(5);
      Serial.println(OCR1A);
    }
  }
  else // Mauvais code
  {
    Serial.println("Po bon");
  }
}

ISR(PCINT0_vect)
{
  if (Distance_Mesure < 50)
  {
  }
}