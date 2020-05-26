#include "core_simulation.h"
#include "mydevices.h"
#include "devices.h"


int main(){
  // creation d'une board
  Board esp8266;

  Detecteurs_Metaux detecteurs_Metaux;

  Lecteur_Rfid lecteur;
  Screen screen;
  Buzzer buzz;


  esp8266.pin(1,detecteurs_Metaux.getCapteur());
  esp8266.i2c(1,detecteurs_Metaux);
  esp8266.set_Rfid(lecteur);

  esp8266.pin(4, buzz);
  

  
  

  

  esp8266.i2c(2,screen);


  // allumage de la carte
  esp8266.run();

  return 0;
}


