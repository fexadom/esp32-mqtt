## Taller demostrativo de uso de MQTT con el ESP32

![Diagrama](diagrama.png)

El objetivo de este taller es demostrar el uso del protocolo de red [MQTT](https://mqtt.org/) para conectar el flujo de información entre dispositivos IoT basados en el microcontrolador [ESP32](https://www.espressif.com/en/products/socs/esp32). La imagen arriba muestra la arquitectura de comunicación objetivo a la que se deberá llegar al final del taller.

### Paso 1: Sensor de distancia

Usar el módulo [ESP32 WROOM](https://www.sparkfun.com/products/15663) y el sensor de distancia [RFD77402](https://www.sparkfun.com/products/retired/14539) para crear un dispositivo que mida distancia a un objeto con precisión milimétrica. Conectar los módulos como se muestra en la imagen:

![ESP32 y RFD77402](esp32_rfd77402.jpg)

Conectar el dispositivo a la computadora y usando el [Arduino IDE](https://www.arduino.cc/en/main/software) cargar el siguiente programa en el dispositivo:
```C++
#include <SparkFun_RFD77402_Arduino_Library.h>

//Objeto de librería sensor
RFD77402 myDistance;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Ejemplo uso RFD77402");

  //Inicializa el sensor
  if (myDistance.begin() == false)
  {
    Serial.println("Error al inicializar el sensor. Revisar cables.");
    while (1); //Freeze!
  }
  Serial.println("Sensor online!");
}

void loop()
{
  //Efectua una medición
  myDistance.takeMeasurement();
  //Obtiene la medición
  unsigned int distance = myDistance.getDistance();
  //Muestre en puerto serial
  Serial.println(distance);
  delay(50);
}
```
Este programa realiza una medición de distancia cada 50 milisegundos, enviando el resultado por el puerto serial. Usar *Serial Monitor* y *Serial Plotter* en el Arduino IDE para revisar las mediciones.

* Para conectar el ESP32 WROOM a su computadora revisar: [ESP32 Thing Plus Hookup Guide](https://learn.sparkfun.com/tutorials/esp32-thing-plus-hookup-guide)
* Para usar las librerías del RFD77402 revisar: [Qwiic Distance Sensor (RFD77402) Hookup Guide](https://learn.sparkfun.com/tutorials/qwiic-distance-sensor-rfd77402-hookup-guide)


### Paso 2: Display de posición
Usar el módulo ESP32 WROOM y el [OLED Transparente](https://www.sparkfun.com/products/15173) para mostrar una animación en el dispositivo como se muestra en la imagen:

![ESP32 y OLED](esp32_lcd.jpg)

Conectar el dispositivo a la computadora y usando el IDE de Arduino, cargar el siguiente código:

```C++
#include "HyperDisplay_UG2856KLBAG01.h"

#define WIRE_PORT Wire

//Distancia máxima
#define MAXDIST 2047

UG2856KLBAG01_I2C myTOLED;

//Posición y radio bolita
uint8_t x;
uint8_t y;
uint8_t r;

int dist;

void posicionarBolita(int distancia)
{
  int x_new = map(distancia, 0, MAXDIST, r+6, myTOLED.xExt);
  if(x_new != x){
    myTOLED.circleClear(x, y, r);
    myTOLED.circleClear(x, y, r-4, true);

    x = (uint8_t) x_new;

     myTOLED.circleSet(x, y, r);
     myTOLED.circleSet(x, y, r-4, true);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Ejemplo bolita movible");

  //Inicializa el OLED en I2C a 400kHz
  WIRE_PORT.begin();
  myTOLED.begin(WIRE_PORT, false, SSD1309_ARD_UNUSED_PIN);
  Wire.setClock(400000);

  //Dibuja la Pared
  myTOLED.rectangleSet(0, 0, 5, myTOLED.yExt, true);

  r=8;
  x=r+6;
  y=myTOLED.yExt/2;
  

  myTOLED.circleSet(x, y, r);             // Dibuja círculo centrado en (x0, y0) con radio r
  myTOLED.circleSet(x, y, r-4, true);     // Dibuja círculo lleno

  dist = 0;
}

void loop() {
  char c;
  if(Serial.available()>0)
  {
    c = Serial.read();
    switch(c)
    {
      case 'a':
        dist -= 50;
        if(dist < 0) dist = 0;
        posicionarBolita(dist);
        break;
      case 'd':
        dist += 50;
        if(dist > MAXDIST) dist = MAXDIST;
        posicionarBolita(dist);
        break;
      default:
        break;
    }    
  }
}
```

Este programa permite mover la bolita en el OLED usando el puerto serial. Desde el Serial Monitor del IDE de Arduino se puede enviar alejar la bolita de la pared con la letra 'd' y acercar a la pared con la letra 'a'.

![Mover la bolita](mueve_bolita.gif)

* Para usar las librerías del OLED Transparente: [Transparent Graphical OLED Breakout Hookup Guide](https://learn.sparkfun.com/tutorials/transparent-graphical-oled-breakout-hookup-guide)

### Paso 3: Configurar broker MQTT
En este paso es necesario configurar al Raspberry Pi como broker MQTT para que sirva de intermediario entre varios dispositivos IoT. Durante el taller se proporcionará a cada estudiante las credenciales para acceder a los Raspberry Pi:
* IP del dispositivo
* Usuario y clave

Usar un cliente SSH como *Putty* para conectarse a la consola del Raspberry Pi e instalar [Mosquitto](https://mosquitto.org/) un broker MQTT código abierto de la siguiente forma:
```bash
# Instala mosquitto MQTT
sudo apt install mosquitto mosquitto-clients
# Registra a mosquitto como servicio
sudo systemctl enable mosquitto.service
```
Verificar la instalación correcta con:
```bash
mosquitto -v
```
Una vez instalado es posible subscribirse y publicar a topicos a traves del broker usando la IP y el puerto 1883.
