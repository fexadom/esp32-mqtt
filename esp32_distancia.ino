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