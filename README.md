# Alarma AVR
TP Integrador con Mateo Josué Torres para Aplicaciones de la Electrónica Digital lII (2024)

### Conexiones
#### LCD y Registro de Desplazamientos
- LCD Con Arduino están en el código
- D0-D7 LCD => QA-QH Registro
- RCLK Registro => SRCLK Registro => "Clk" Arduino
- OE Registro => GND
- SRCLR Registro => VCC
- SER Registro => "Data" Arduino
- RW, VDD => VCC
- V0 => Resistencia 400R => GND Y Resistencia 1K VCC o una caída de 1,6V en este pin, cuanto mas cerca de gnd mas se ilumina

### Objetivo 

Desarrollar un sistema de alarma aplicando los conocimientos adquiridos.

### Tareas a dividir

#### Protocolos de comunicación
- [x] LCD (Nico)
- [ ] DHT11 (Nico)
- [ ] Keypad (Matuto)
- [ ] Buzzer (Nico) piupiupiupiu
- [ ] Sensor Perimetral (Botones) (Mateo)

#### Configuración
- [ ] Usuarios (Mateo)
- [ ] Claves de usuario (Mateo)
- [ ] Activar y desactivar distintas alarmas (Pedir clave) (Nico)
- [ ] Hora (Mateo)

#### Llamado de alarma
- [ ] Activa buzzer 10s
- [ ] Guarda quien Desactivo la alarma
- [ ] Guarda quien la activo y a que hora

#### Tiempo (Mateo)
- [x] Hora andando

