//Cosas del LCD
#define F_CPU 8000000

// Pines lcd
#define clk PB1
#define data PB0
#define rs PB2
#define e PB3

// Pines Sensores
#define sensorPerimetral PB4
#define sensorInterno PB5

#define alarma PC4

// Parametros usuarios
#define PASSWORD_LENGTH 4
#define USERS 3

// Parametros del keypad
char key;
char prevKey = 'E';

// Una contrasena por usuario
char *passwords[USERS] = {"1234", "5678", "0812"}; // En realidad habria que implementar un sistema de hasheo...

// Configuraciones y estados de alarma
uint8_t mode = 2; //0 Desactivada, 1 Perimetral, 2 Total
uint8_t state = 0; //0 Sonido apagado, 1 Sonando

uint8_t prevMode = 2; //0 Desactivada, 1 Perimetral, 2 Total
uint8_t prevState = 0; //0 Sonido apagado, 1 Sonando

uint8_t hourState = 0; // 1 si los segundos son multiplo de 5
uint8_t prevHourState = 0;

struct hour
{
  int hrs;
  int min;
  int sec;
};

struct hour time = {0, 0, 0};


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
        //updateScreen();
      }
      else if (key == 'A')
      {
        configureMode();
        updateScreen();
      }
      else if (key == 'C')
      {
        configurePasswords();
        updateScreen();
      }
    }
  }
}

// Aca abajo van las funciones de los otros archivos para subir a tinkercad
