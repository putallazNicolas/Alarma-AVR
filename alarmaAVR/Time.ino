struct hour
{
  int hrs;
  int min;
  int sec;
};

struct hour time = {0, 0, 0};

void tiempoDeclare()
{
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TCCR1A = 0;
  OCR1A = 15624; // Valor para interrumpir cada un segundo = 15624
  TIMSK1 = (1 << OCIE1A);
  sei();
}

void printTime()
{
  Serial.print(time.hrs);
  Serial.print(":");
  Serial.print(time.min);
  Serial.print(":");
  Serial.print(time.sec);
  Serial.println();
}

ISR (TIMER1_COMPA_vect)
{
//  printTime();

  time.sec++;

  if (time.sec >= 60)
  {
    time.sec = 0;
    time.min++;
    if (time.min >= 60)
    {
      time.min = 0;
      time.hrs++;
    }
  }
}