// users.ino

void configurePasswords()
{
  lcd_clear();
  lcd_write("Que usuario sos?");

  char key;
  while (1)
  {
    key = checkKeypad();
    if (key)
    {
      break;
    }
  }

  if (key == '1' || key == '2' || key == '3')
  {
    askPassword();
  }

  lcd_clear();
  lcd_write("Nueva Contrasena");
  lcd_set_cursor(0, 1);

  char input[PASSWORD_LENGTH];
  lcd_show_cursor(1, 1);

  for (int i = 0; i < PASSWORD_LENGTH; i++)
  {
    char keyB;
    while (1)
    {
      keyB = checkKeypad();
      if (keyB)
      {
        break;
      }
    }
    lcd_write_char(keyB);
    input[i] = keyB;
  }

  Serial.print(passwords[2]);
  Serial.print("\t");
  Serial.print(key);
  Serial.print("\t");
  Serial.println(input);

  if (key == '0')
  {
    strncpy(passwords[0], input, PASSWORD_LENGTH);
  }
  if (key == '1')
  {
    strncpy(passwords[1], input, PASSWORD_LENGTH);
  }
  if (key == '2')
  {
    strncpy(passwords[2], input, PASSWORD_LENGTH);
  }

  Serial.print(passwords[2]);
  Serial.print("\t");
  Serial.print(key);
  Serial.print("\t");
  Serial.println(input);

  return;
}
