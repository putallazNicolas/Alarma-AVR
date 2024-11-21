char keys[4][4] =
{
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

char checkKeypad()
{
  for (int r = 0; r < 4; r++)
  {
    PORTC = ~(1 << r);
    for (int c = 4; c < 8; c++)
    {
      if (!(PIND & (1 << c)))
      {
        char keyss = checkReps(r, c - 4);
        return keyss;
      }
    }
    PORTC = 0b000000;
  }
  prevKey = '\0';
  return '\0';
}

void setupKeypad()
{
  DDRC = 0b001111;
  DDRD = 0b00000000;
  PORTC = 0b111111;
  PORTD = 0b11110000;
}

char checkReps(int r, int c)
{
  int tiempoActual = time.sec;
  if(keys[r][c] != prevKey) //La tecla presionada es distinta a la anterior (dejó de ser rebote)
  {
    prevKey = keys[r][c];
    return keys[r][c];
  }
  else if(keys[r][c] == prevKey && tiempoActual != time.sec) //La tecla que fue antes presionada, después de un segundo sigue presionada (no es rebote)
  {
    prevKey = keys[r][c];
    return keys[r][c];
  }
  else if(keys[r][c] == prevKey && tiempoActual == time.sec) //La tecla sigue presionada en el mismo segundo (es rebote)
  {
    prevKey == keys[r][c];
    return '\0';
  }
}
