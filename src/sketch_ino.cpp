#include <unistd.h>
#include "core_simulation.h"
#include "base_donnees.h"



// la fonction d'initialisation d'arduino
void Board::setup(){
  // on configure la vitesse de la liaison
  Serial.begin(9600);
// on fixe les pin en entree et en sorite en fonction des capteurs/actionneurs mis sur la carte

  pinMode(1,INPUT);
  pinMode(4, OUTPUT);


}

// la boucle de controle arduino
void Board::loop(){

    char buf[100];
    char buf_Rfid[4];
    char buf2[100];

    bus.requestFrom(1,buf,100);
    Serial.println(buf);
    //bus.write(2,buf,100);
    if (digitalRead(1) && strcmp( buf, "Boutton ON , objet metallique porte  " )==0){
        digitalWrite(4, HIGH);
        cout<< "Buzzerrrrrrrrrrrrrrrrrr"<< endl;
}
    
    else if(digitalRead(1) && strcmp( buf, "Boutton ON , aucun objet metallique, veuillez avancer svp" )==0  ){

            // si on est rentre dans ce if, ca veut dire qu'un employe est sur le point de passer son badge
            // on met donc sck a 1 pour signaler au lecteur rfid qu'a son prochain run,
            //il doit simuler une nouvelle sequence pour le tag rfid, et si sck etait deja a 1, alors on le laisse a 1
            if (sck ==0){
                sck =1;
            }
            else if (sck == 1){

                for(int i = 0; i<3;i++){
                        buf_Rfid[i] = miso[i];

                }

                //vider_Chaine(buf2);
                sprintf(buf2," Board, sequence tag : %s ",buf_Rfid);
                Serial.println(buf2);
                rechercher_base_donnees(buf_Rfid);


            }
    }
            // si on rentre dans ce else if, cela veut dire que dans le run d'avant une personne arrive
            //au nivau du lecteur, mais que la il n'y a plus personne qui compte arriver, donc on remet sck a 0 comme ca le
            // lecteur verra (quand son thread arrivera au prochain tour) que personne n'arrive

    else if (sck == 1){

                for(int i = 0; i<3;i++){
                        buf_Rfid[i] = miso[i];

                }

                sck = 0;

                //vider_Chaine(buf2);
                sprintf(buf2," Board, sequence tag : %s ",buf_Rfid);

                Serial.println(buf2);

                rechercher_base_donnees(buf_Rfid);

            }
    // 2
    sleep(4);

  }


// 2 problemes a corriger : la synchronisation des threads --> 2 variables statiques : au capteur et au lecteur rfid
//
