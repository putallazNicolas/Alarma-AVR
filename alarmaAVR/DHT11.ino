#define DHT11 PD2

/*
//COPIAR EN MAIN
typedef struct
{
  int humedad_ent;
  int humedad_dec;
  int temperatura_ent;
  int temperatura_dec;
  int checksum;
}
dhtData;

dhtData dht11 = {0, 0, 0, 0, 0};
*/




/*
int DHT(void) // Retorna 0 si hubo un error, retorna 1 si fue realizado correctamente
{
  // Empieza la comunicación
  DDRD |= (1 << DHT11); 
  PORTD &= ~(1 << DHT11);
  _delay_ms(18);
  PORTD |= (1 << DHT11);
  DDRD &= ~(1 << DHT11);
  _delay_us(40);

  // El DHT11 chequea que se recibieron bien los datos
  if (PIND & (1 << DHT11))
  {
    //Serial.println("Checkpoint 1");
    _delay_ms(20);
    return 0;
  }
  _delay_us(80);
  if (!(PIND & (1 << DHT11)))
  {
    //Serial.println("Checkpoint 2");
    _delay_ms(20);
    return 0;
  }

  // El DHT11 empieza a enviar datos, empezando por el entero de la humedad
  dht11.humedad_ent = leerDHT();
  dht11.humedad_dec = leerDHT();
  dht11.temperatura_ent = leerDHT();
  dht11.temperatura_dec = leerDHT();
  dht11.checksum = leerDHT();
  
  if (dht11.checksum == dht11.humedad_ent + dht11.humedad_dec + dht11.temperatura_ent + dht11.temperatura_dec)
  {
    return 1;
  }
  else
  {
    //Serial.println("Checkpoint 3");
    _delay_ms(20);
    return 0;
  }
  return 1;
}

int leerDHT(void)
{
  int valor = 0;
  //while (PIND & (1 << DHT11));
  for (int i = 0; i < 8; i++)
  {
    if (PIND & (1 << DHT11)) 
    {
      //Serial.println("Checkpoint 4");
      _delay_ms(20);
      return 0;
    }
    
    _delay_us(50);
    
    if (!(PIND & (1 << DHT11)))
    {
      //Serial.println("Checkpoint 5");
      _delay_ms(20);
      return 0;
    }
    
    _delay_us(28);
    
    if (!(PIND & (1 << DHT11)))
    {
      valor &= ~(1 << (7 - i));
    }
    else
    {
      valor |= (1 << (7 - i));
      while (!(PIND & (1 << DHT11))); // Espera a que baje
    }
  }
  return valor;
}

*/
