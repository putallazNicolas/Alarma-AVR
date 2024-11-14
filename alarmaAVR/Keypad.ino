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
    PORTB = ~(1 << r);
    _delay_ms(50);
    //Serial.print("PORTB: ");
    //Serial.println(PINB, BIN);
    for (int c = 4; c < 7; c++)
    {
	    //Serial.print("PORTD: ");
      //Serial.println(PIND, BIN);
      if (!(PIND & (1 << c)))
      {
        return keys[r][c - 4];
      }
    }
    PORTB = 0b00000000;
  }
  return '\0';
}

void setupKeypad()
{
    // Configura las columnas (pines 4-7) como salidas
    DDRD = 0b00000000;   // Configura los pines 4, 5, 6 y 7 como salidas
    PORTD = 0b00000000;  // Los pone en alto para evitar falsos contactos

    // Configura las filas (pines 8-11) como entradas con pull-up
    DDRB = 0b001111;   // Configura los pines 8, 9, 10 y 11 como entradas
    PORTB = 0b000000;  
}