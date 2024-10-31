int flag = 0;

char keys[4][4] =
{
{ '1','2','3', 'A' },
{ '4','5','6', 'B' },
{ '7','8','9', 'C' },
{ '#','0','*', 'D' }
};

char checkKeypad()
{
  for (int c = 0; c < 4; c++)
  {
    PORTD &= ~(1 << c + 2);
    
    for(int r = 0; r < 4; r++)
    {
      if(r < 2)
      {
        if (!(PIND & (1 << (r + 6)))) 
        {
          while (!(PIND & (1 << (r + 6))));
          PORTD |= (1 << c);
          return keys[r][c];
          flag = 0;
        }
      }
      else
      {
        if (!(PINB & (1 << (r - 2)))) 
        {
          while (!(PINB & (1 << (r - 2))));
          PORTB |= (1 << c);
          return keys[r][c];
          flag = 1;
        }
      }
    }
    if(flag == 0)
  {
    PORTD |= (1 << c);
  }
  else
  {
    PORTB |= (1 << c);
  }
    return 0;
  }
}

void setupKeypad()
{
  //columnas
  DDRD = 0b00111100;
  DDRB = 0b000000;
  //filas
  PORTD = 0b11111100;
  PORTB = 0b111111;
}