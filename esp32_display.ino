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