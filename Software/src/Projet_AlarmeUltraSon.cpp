#include "Projet_AlarmeUltraSon.hpp"

int Sortie_Led = 3;
int Sortie_Buzzer = 2;
int Entree_BP = 7; 
int URECHO = 9;      // PWM Output 0-25000US,Every 50US represent 1cm
int URTRIG = 8;      // PWM trigger pin
int sensorPin = A0;  // select the input pin for the potentiometer
int sensorValue = 0; // variable to store the value coming from the sensor
unsigned int Distance_Mesure = 0;
bool alarme = 0;
char code_secret[5] = "4582";

char code[5];


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


void Frequence(int x)
{
  if(x == 0)
  {
    DDRB &= ~(1 << Sortie_Buzzer);    // Arret du signal
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
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Mode Fast PWM et Prescaler à 8
  TCCR1A |= (1 << COM1B0);  
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


void Init_clavier()
{
  DDRD |= (1 << PD6) | (1 << PD2) | (1 << PD3) | (1 << PD4); // Config lignes = sorties
  DDRC &= ~((1 << PC3) | (1 << PC4) | (1 << PC5));           // Config colonnes = entrées
  PORTC |= (1 << PC3) | (1 << PC4) | (1 << PC5);             // Pull-up colonnes
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
    Serial.println("Code bon");
    alarme ^= 1;
  }
  else // Mauvais code
  {
    Serial.println("Po bon");
  }
}


void Init_BP(void)
{
  PORTD |= (1 << Entree_BP);
  DDRB |= (1 << Entree_BP);
  // PIND permet d'avoir l'info si BP est appuyé
}


void Definir_code(void)
{
  while(PIND & (1 << Entree_BP))
  {
    for(int i; i < 4; i++)
    {
      code_secret[i] = Lecture_clavier();
    }
  }
}


void Init_Can()
{
  ADMUX |= (1 << REFS0); //permet de choisir la valeur du VCC soit 5V (ADC varie entre 0 et 1023)
  ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3)); // Canal A0
  ADCSRA |= (1 << ADEN); //ACTIVE ADC (allume le CAN)
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1)  | (1 << ADPS2); //prescaler a 128 (regle fréquence du CAN à 125KHz)
}


int Lire_ADC()
{
  ADCSRA |= (1 << ADSC);  //Commence conversion
  do{}while(ADCSRA & (1 << ADSC)); //Attendre fin conversion
  uint16_t x_val = ADCL;
  x_val += (ADCH << 8);
  return x_val;
}


void Init_interruption()
{
  DDRB &= ~(1 << PB1);  //PB1 en entrée
  PCICR |= (1 << PCIE0);  //Autorise interruption PORTB
  PCMSK0 |= (1 << PCINT1);  //Active PB1
}



/*ISR(PCINT0_vect)
{
  if(alarme == 1)
  {
    if(lecture_clavier() != 0)
    {
      Lire_code(code);
      Verifier_code(code);
    }
    Mesure_distance();
    if(Distance_Mesure < 50){
      for(int i = 100; i < 10000; i += 20)
      {
        frequence(i);
        _delay_ms(5);
        
      }
      for(int i = 10000; i > 100; i -= 20)
      {
        frequence(i);
        _delay_ms(5);
      }
    }
    frequence(0);
  }
  if((alarme == 0) & (PIND & (1 << Entree_BP)))
  {
    Definir_code();
  }
}
*/