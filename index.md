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

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/fexadom/esp32-mqtt/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
