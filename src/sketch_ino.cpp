#include <unistd.h>
#include "core_simulation.h"

// la fonction d'initialisation d'arduino
void Board::setup(){
  // on configure la vitesse de la liaison
  Serial.begin(9600);
// on fixe les pin en entree et en sorite en fonction des capteurs/actionneurs mis sur la carte
  pinMode(1,INPUT);
  pinMode(0,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  pinMode(4,INPUT);
}

// la boucle de controle arduino
void Board::loop(){
  char buf[100];
  int val;
  int val_boutton;
  char etat_boutton[4];
  static int cpt=0;
  //static int bascule=0;
  int i=0;
  for(i=0;i<10;i++){
    // lecture sur la pin 1 : capteur de temperature
    val=analogRead(2);
    val_boutton = digitalRead(4);
    if (val_boutton == 1){memcpy(etat_boutton,"ON",4);}
    else {memcpy(etat_boutton,"OFF",4);}
    sprintf(buf,"luminosite %d, bouton %s",val, etat_boutton);
    Serial.println(buf);

    if(cpt%5==0){
        // tous les 5 fois on affiche sur l ecran la luminosite
      sprintf(buf,"%d, %d",val,val_boutton);
      bus.write(1,buf,100);
    }
    cpt++;
    sleep(1);
  }
// on eteint et on allume la LED
  if(val_boutton)
    digitalWrite(3,HIGH);
  else
    digitalWrite(3,LOW);
  //bascule=1-bascule;

}
