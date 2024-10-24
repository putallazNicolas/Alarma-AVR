#define DHT11 PD2

uint8_t *DHT_DDR = &DDRD;
uint8_t *DHT_PORT = &PORTD;
uint8_t *DHT_PIN = &PIND;

int DHT(void) // Retorna 0 si hubo un error, retorna 1 si fue realizado correctamente
{
  int humedad_ent = 0;
  int humedad_dec = 0;
  int temperatura_ent = 0;
  int temperatura_dec = 0;
  int checksum = 0;

  // Empieza la comunicación
  *DHT_DDR |= (1 << DHT11); 
  *DHT_PORT &= ~(1 << dht11);
  _delay_ms(18);
  *DHT_PORT |= (1 << DHT11);
  _delay_us(30);

  // El DHT11 chequea que se recibieron bien los datos
  *DHT_DDR &= ~(1 << DHT11);
  if (DHT_PIN & (1 << DHT11) == 1) return 0;
  _delay_us(80);
  if (DHT_PIN & (1 << DHT11) == 0) return 0;
  _delay_us(80);

  // El DHT11 empieza a enviar datos, empezando por el entero de la humedad
  humedad_ent = leerDHT();
  humedad_dec = leerDHT();
  temperatura_ent = leerDHT();
  temperatura_dec = leerDHT();
  checksum = leerDHT();
}

int leerDHT(void)
{
  int valor = 0;
  for (int i = 0; i < 8; i++)
  {
    if (DHT_PIN & (1 << DHT11) == 1) return 0;
    _delay_us(50);
    if (DHT_PIN & (1 << DHT11) == 0) return 0;
    _delay_us(28);
    if (DHT_PIN & (1 << DHT11) == 0)
    {
      valor &= ~(1 << (7 - i));
    }
    else
    {
      valor |= (1 << (7 - i));
      while (DHT_PIN & (1 << DHT11) == 0); // Espera a que baje
    }
  }
  return valor;
}