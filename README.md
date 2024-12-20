# Alarma AVR
TP Integrador con Mateo Josué Torres para Aplicaciones de la Electrónica Digital III (2024)

[Proyecto en tinkercad](https://www.tinkercad.com/things/1oUHeG1ec9G-alarma-avr)

### Objetivo 

Desarrollar un sistema de alarma aplicando los conocimientos adquiridos.

### Esquematico de conexión

![esquematico](https://github.com/user-attachments/assets/c73fe07e-00ac-47fd-879e-30ee1953cfdb)

### Descripción de archivos

#### Todos terminan en '.ino'

- AlarmaAVR: main y declaracicon de variables globales.
- DHT11: Comunicación con el sensor de temperatura y humedad "DHT11"
- LCD1602_tomi: Comunicación con el LCD 16x2 a través de un registro de desplazamiento. Funciones realizadas por Tomás Paternó, con pequeñas modificaciones.
- Keypad: Comunicación con el teclado matricial 4x4.
- Sensors: Comunicación con los sensores y distintas configuraciones relacionadas con estos.
- Users: Configuración relacionada a los usuarios.
- Time: Manejo de la hora.
- Alarm: Comunicación con el buzzer y distintas configuraciones de activacion de distintos modos de alarma.

### Tareas a dividir

#### Protocolos de comunicación
- [x] LCD (Nico)
- [ ] DHT11 (Nico)
- [x] Keypad (Matuto)
- [x] Buzzer básico (1 o 0) (Nico)
- [ ] Buzzer Avanzado (Nico)
- [x] Sensores (Botones) (Mateo)

#### Configuración
- [x] Usuarios (Mateo)
- [x] Claves de usuario (Mateo)
- [ ] Guardar distintas claves de usuario en la eeprom.
- [x] Activar y desactivar distintas alarmas (Pedir clave) (Nico)
- [ ] Hora (Mateo)

#### Llamado de alarma
- [x] Activa buzzer
- [x] Se apaga la sirena si ponen bien la clave
- [ ] Guarda quien Desactivo la alarma
- [ ] Guarda a que hora se activo (historial)

#### Tiempo (Mateo)
- [x] Hora andando
- [ ] Guardar la hora en la eeprom para cuando se apaga el dispositivo seguir al volverlo a encender

