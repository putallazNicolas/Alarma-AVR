#include "definiciones.h"

int main(void)
{
  tiempoDeclare();

  lcd_init();
  lcd_show_cursor(0, 0);

  setupSensors();
  setupKeypad();

  updateScreen();

  Serial.begin(9600);
  
  while(1)
  {
    key = checkKeypad();

    checkSensors();
    checkState();

    uint8_t changed = checkChanges();

    if (changed)
    {
      updateScreen();
    }
  
    if (key)
    {
      if (key > '0' && key < '9')
      {
        deactivateAlarm();
      }
      else if (key == 'A')
      {
        configureMode();
      }
      else if (key == 'C')
      {
        configurePasswords();
        updateScreen();
      }
    }
  }
}

// sensors.ino

// alarm.ino

// LCD1602.ino

//keypad.ino

//Time.ino


