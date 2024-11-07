char keys[4][4] =
{
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '#', '0', '*', 'D' }
};

char checkKeypad()
{
  for (int r = 0; r < 4; r++)
  {
    PORTB &= ~(1 << r);
    _delay_ms(50);
    for (int c = 4; c < 8; c++)
    {
      if ((PORTD | (1 << c)) == 0)
      {
        return keys[r][c - 4];
      }
    }
  }
  return '\0';
}

void setupKeypad()
{
  DDRB = 0b001111;
  DDRD = 0b00000000;
  PORTB = 0b111111;
  PORTD = 0b00000000;
}
