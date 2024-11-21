#include <Keypad.h>

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

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char key; // NO BORRAR AL CAMBIAR A AVR

// Una contrasena por usuario
char *passwords[3] = {"1234", "1234", "1234"};

// Configuraciones y estados de alarma
uint8_t mode = 0; //0 Desactivada, 1 Perimetral, 2 Total
uint8_t state = 0; //0 Sonido apagado, 1 Sonando

void setup()
{
  Serial.begin(9600);
  lcd_init();
  lcd_show_cursor(0, 0);
}
  
void loop()
{
  key = customKeypad.getKey();
  mode = 1;
  checkSensors();
  checkState();
  lcd_clear();
  lcd_write("Estado: ");
  lcd_write_char(state + 48); // El +48 es por que al estar imprimiendo un numero el codigo ascii lo toma como char e imprime NULL si es 0, entonces le sumo 48 (0 en ascii)
  lcd_set_cursor(0, 1);
  lcd_write("Modo: ");
  lcd_write_char(mode + 48);
  
  if (key)
  {
    desactivarAlarma();
  }
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

void desactivarAlarma()
{
  if (state == 0)
  {
    return;
  }
  else
  {
    state = 0;
  }
}

void setupSensores()
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

