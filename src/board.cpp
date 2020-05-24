#include "core_simulation.h"
#include "mydevices.h"
#include "devices.h"


int main(){
  // creation d'une board
  Board esp8266;

  Detecteurs_Metaux detecteurs_Metaux;

  Lecteur_Rfid lecteur;
  Screen screen;


    esp8266.pin(1,detecteurs_Metaux.getCapteur());
  esp8266.i2c(1,detecteurs_Metaux);
  esp8266.set_Rfid(lecteur);
  esp8266.run();

  esp8266.i2c(2,screen);







  // achat des senseurs et actionneurs
  /*AnalogSensorTemperature temperature(DELAY,TEMP);
  DigitalActuatorLED led1(DELAY);
  IntelligentDigitalActuatorLED led2(DELAY);
  I2CActuatorScreen screen;
  AnalogSensorLuminosity luminosite(DELAY);
  ExternalDigitalSensorButton button;*/

  // branchement des capteurs actionneurs
 /* esp8266.pin(1,temperature);
  esp8266.pin(0,led1);
  esp8266.pin(3,led2);
  esp8266.pin(2,luminosite);
esp8266.pin(4,button);
  */


  // allumage de la carte


  return 0;
}


