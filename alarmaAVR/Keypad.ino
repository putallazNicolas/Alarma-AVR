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
    PORTB = ~(1 << r);
    for (int c = 4; c < 8; c++)
    {
      if (!(PIND & (1 << c)))
      {
        return keys[r][c - 4];
      }
    }
    PORTB = 0b000000;
  }
  return '\0';
}

void setupKeypad()
{
  DDRB = 0b001111;
  DDRD = 0b00000000;
  PORTB = 0b111111;
  PORTD = 0b11110000;
}
