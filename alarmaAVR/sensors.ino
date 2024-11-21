void setupSensors()
{
  // Configurar como entradas
  DDRB &= ~(1 << sensorPerimetral);
  DDRB &= ~(1 << sensorInterno);

  // Configurar sin pullup
  PORTB &= ~(1 << sensorPerimetral);
  PORTB &= ~(1 << sensorInterno);

  //Buzzer como salida
  DDRC |= (1 << alarma);
}

void checkSensors() //0 Desactivada, 1 Perimetral, 2 Total
{
  // Chequear estado sensores
  uint8_t perimetralState = PINB & (1 << sensorPerimetral);
  uint8_t internoState = PINB & (1 << sensorInterno);

  if (mode == 0)
  {
    return;
  }
  else if (mode == 1)
  {
    if (perimetralState)
    {
      state = 1;
    }
  }
  else if (mode == 2)
  {
    if (perimetralState || internoState)
    {
      state = 1;
    }
  }
}

void checkState()
{
  if (state)
  {
    PORTC |= (1 << alarma);
  }
  else
  {
    PORTC &= ~(1 << alarma);
  }
}

uint8_t checkChanges()
{
  if (state != prevState)
  {
    prevState = state;
    return 1;
  }
  
  if (mode != prevMode)
  {
    prevMode = mode;
    return 1;
  }

  if (time.sec % 5 == 0)
  {
    hourState = 1;
  }
  else
  {
    hourState = 0;
  }

  if (hourState != prevHourState)
  {
    prevHourState = hourState;
    return 1;
  }

  return 0;
}

void updateScreen()
{
  if (time.sec % 5 == 0)
  {
    lcd_clear();
    char hourString[16];
    sprintf(hourString, "%02d:%02d:%02d", time.hrs, time.min, time.sec);
    lcd_write("Hora: ");
    lcd_write(hourString);
  }
  else
  {
    lcd_clear();

    if (mode == 0) // 0 Desactivada, 1 Perimetral, 2 Total
    {
      lcd_write("Desactivada");
    }
    else if (mode == 1)
    {
      lcd_write("Perimetral");
    }
    else if (mode == 2)
    {
      lcd_write("Armado Total");
    }

    lcd_set_cursor(0, 1);

    if (state == 0) // 0 Sonido apagado, 1 Sonando
    {
      lcd_write("No hay intrusos");
    }
    else if (state == 1)
    {
      lcd_write("ALARMA ACTIVADA");
    }
  }
}
