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

#define PASSWORD_LENGTH 4
#define USERS 3

char key; // NO BORRAR AL CAMBIAR A AVR

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

  key -= 48; // Trato al char como un int

  lcd_clear();
  lcd_write("Nueva Contrasena");
  lcd_set_cursor(0, 1);

  char input[PASSWORD_LENGTH];

  for (int i = 0; i < PASSWORD_LENGTH; i++)
  {
    lcd_show_cursor(1, 1);
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
  lcd_show_cursor(0, 0);

  if (key >= '0' && key <= '2')
  {
    int index = key - '0';  // Convierte '0', '1', '2' a 0, 1, 2
    strncpy(passwords[index], input, PASSWORD_LENGTH);
    passwords[index][PASSWORD_LENGTH - 1] = '\0';  // Asegura el terminador nulo
  }

  return;
}

// sensors.ino

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

  for (int i = 0; i < PASSWORD_LENGTH; i++)
  {
    lcd_show_cursor(1, 1);
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

// LCD1602.ino

void lcd_enable_pulse()
{
	PORTB &= ~(1<<e);
	_delay_ms(1);
	PORTB |= (1<<e);
	_delay_ms(1);
	PORTB &= ~(1<<e);
}

void lcd_load_info(unsigned char Info)
{
	
	//Necesito darle un pulso de clock extra ya que el sn74hc595 necesita un pulso para guardar el valor y otro un poco mas tarde para mostrarlo, 
	//es por eso que en el final hace falta darle un clock mas
	
	for(unsigned char i = 0; i < 8; i ++)
	{
		if((Info & (1<<(i))) == (1<<(i))){PORTB |= (1<<data);}
		else{PORTB &= ~(1<<data);}
		PORTB |= (1<<clk);
		PORTB &= ~(1<<clk);
	}
	PORTB |= (1<<clk);
	PORTB &= ~(1<<clk);
}

void lcd_clear()
{
	PORTB &= ~(1<<rs);
	unsigned char Clear = 0b10000000;
	lcd_load_info(Clear);
	lcd_enable_pulse();
  lcd_set_cursor(0, 0);
}

void lcd_show_cursor(int Cursor, int blinking)
{
	unsigned char ParpadearCursor = 0b00110000;
	if(Cursor == 1){ParpadearCursor |= (1<<6);}
	if(blinking == 1){ParpadearCursor|= (1<<7);}
	lcd_load_info(ParpadearCursor);
	lcd_enable_pulse();
}

void lcd_startup()
{
	_delay_ms(1000);
	PORTB &= ~(1<<e);
	PORTB &= ~(1<<rs);
	
	//Configura para trabajar con 8 bits y 2 filas
	unsigned char TrabajarCon8Bits = 0b00011100;
	lcd_load_info(TrabajarCon8Bits);
	lcd_enable_pulse();
	lcd_clear();
}

void lcd_set_cursor(int Row, int Col)
{
	//Filas: De 0 a 15 Columnas: De 0 a 1
	
	PORTB &= ~(1<<rs);
	
	unsigned char SetColumn= 0b00000001;
	
	if(Col == 1){SetColumn |= (1<<1);}
	lcd_load_info(SetColumn);
	lcd_enable_pulse();
	
	unsigned char MoveCursor = 0b00101000;
	lcd_load_info(MoveCursor);
	if(Row != 0)
	{
		unsigned char cont = 0;
		while(Row > cont){lcd_enable_pulse();cont++;}	
	}
	
}

void lcd_write(char Text[])
{
	PORTB |= (1<<rs);
	for(int k = 0; k < strlen (Text); k++)
	{
		
		unsigned char Num = Text[k];
		unsigned char Binary= 0b00000000;
		for(int p = 0; p < 8; p++)
		{
			Binary |= (Num % 2<<(7-p));
			Num = Num / 2;
		}
	
		lcd_load_info(Binary);
		lcd_enable_pulse();
	}
	
}

void lcd_write_char(char Text)
{
	PORTB |= (1<<rs);
		
	unsigned char Num = Text;
	unsigned char Binary= 0b00000000;
	for(int p = 0; p < 8; p++)
	{
		Binary |= (Num % 2<<(7-p));
		Num = Num / 2;
	}

	lcd_load_info(Binary);
	lcd_enable_pulse();
}

void write_special_character(int Pointer)
{
	unsigned char cont = 0;
	unsigned char CaracterCode = 0b00000000;
	while(Pointer > 0 && cont < 3)
	{
		CaracterCode |= (Pointer % 2<<(7-cont));
		Pointer = Pointer / 2;
		cont++;
	}
	
	PORTB |= (1<<rs);
	lcd_load_info(CaracterCode);
	lcd_enable_pulse();
}

void CreateSpecialCaracter(unsigned char Pointer, unsigned char HexadecimalIcon[8])
{
	unsigned char CGRAM_ADRESS = 0b01000000;
	Pointer = Pointer<<3;
	CGRAM_ADRESS |= Pointer;

	for(unsigned char i = 0; i < 8; i ++)
	{
		CGRAM_ADRESS &= 0b11111000;
		CGRAM_ADRESS |= i;
		
		unsigned char Pass = 0;
		unsigned char Icon = 0;
		for(unsigned char o = 0; o < 8; o ++)
		{
			if((CGRAM_ADRESS & (1<<o)) == (1<<o)){Pass |= (1<<(7-o));}
			else{Pass &= ~(1<<(7-o));}
			if((HexadecimalIcon[i] & (1<<o)) == (1<<o)){Icon |= (1<<(7-o));}
			else{Icon &= ~(1<<(7-o));}
		}
		PORTB &= ~(1<<rs);
		lcd_load_info(Pass);
		lcd_enable_pulse();
		PORTB |= (1<<rs);
		lcd_load_info(Icon);
		lcd_enable_pulse();
	}
	
	PORTB &= ~(1<<rs);
	unsigned char SetDDRAM_ADRESS = 0b00000001;
	lcd_load_info(SetDDRAM_ADRESS);
	lcd_enable_pulse();
}

void lcd_init()
{
  DDRB |= (1<<clk);
	DDRB |= (1<<data);
	DDRB |= (1<<rs);
	DDRB |= (1<<e);
	
	PORTB &= ~(1<<clk);
	PORTB &= ~(1<<data);
	PORTB &= ~(1<<rs);
	PORTB &= ~(1<<e);
	lcd_startup();
	lcd_clear();

	unsigned char Flecha[8] = {0x0, 0x4, 0x2, 0x1F, 0x2, 0x4, 0x0, 0x0};
	unsigned char CaritaFeliz[8] = {0x0, 0xA, 0xA, 0x0, 0x11, 0xE, 0x0, 0x0};
	unsigned char HO[8] = {0x1F, 0x4, 0x4, 0x1F, 0x0, 0x1F, 0x11, 0x1F};
	unsigned char LA[8] = {0x1F, 0x10, 0x10, 0x0, 0x1E, 0x9, 0x9, 0x1E};
}

//keypad.ino

char prevKey = 'E';

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

//Time.ino



void tiempoDeclare()
{
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TCCR1A = 0;
  OCR1A = 15624; // Valor para interrumpir cada un segundo = 15624
  TIMSK1 = (1 << OCIE1A);
  sei();
}

void printTime()
{
  Serial.print(time.hrs);
  Serial.print(":");
  Serial.print(time.min);
  Serial.print(":");
  Serial.print(time.sec);
  Serial.println();
}

ISR (TIMER1_COMPA_vect)
{
//  printTime();

  time.sec++;

  if (time.sec >= 60)
  {
    time.sec = 0;
    time.min++;
    if (time.min >= 60)
    {
      time.min = 0;
      time.hrs++;
      if (time.hrs == 24)
      {
        time.hrs = 0;
      }
    }
  }
}