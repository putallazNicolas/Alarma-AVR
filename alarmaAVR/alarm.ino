// alarm.ino

void configureMode() //0 Desactivada, 1 Perimetral, 2 Total
{
  lcd_clear();
  lcd_write("0:Off  2: Total");
  lcd_set_cursor(0, 1);
  lcd_write("1: Perimetral");

  char key;
  while (1)
  {
    key = checkKeypad();
    if (key)
    {
      break;
    }
  }

  if (key == '0' || key == '1' || key == '2')
  {
    askPassword();
  }
  
  if (key == '0')
  {
    mode = 0;
  }
  else if (key == '1')
  {
    mode = 1;
  }
  else if (key == '2')
  {
    mode = 2;
  }

  return;
}

void deactivateAlarm()
{
  if (state == 0)
  {
    return;
  }
  else
  {
    askPassword();
  }
}

void askPassword()
{
  lcd_clear();
  lcd_set_cursor(0, 0);
  lcd_write("Contrasenia:");
  lcd_set_cursor(0, 1);

  char input[PASSWORD_LENGTH];

  lcd_show_cursor(1, 1);

  for (int i = 0; i < PASSWORD_LENGTH; i++)
  {
    char key;
    while (1)
    {
      key = checkKeypad();
      if (key)
      {
        break;
      }
    }
    lcd_write_char(key);
    input[i] = key;
  }

  uint8_t isCorrect = checkPassword(input);

  if (isCorrect)
  {
    lcd_clear();
    lcd_show_cursor(0, 0);
    state = 0;
  }
  else
  {
    askPassword();
  }
}

uint8_t checkPassword(char *input) // Estaria bueno implementar un hasheo
{
  for (int i = 0; i < USERS; i++)
  {
    if(strCompare(input, passwords[i], PASSWORD_LENGTH))
    {
      return 1;
    }
  }
  return 0;
}

int strCompare(char *string1, char *string2, int length) // ERA LA CLAVE HACERLO MANUAL, NO ANDA USANDO strcmp DE LA LIBRERIA string.h
{
  for (int i = 0; i < length; i++)
  {
    if (string1[i] != string2[i])
    {
      return 0;
    }
  }
  return 1;
}
