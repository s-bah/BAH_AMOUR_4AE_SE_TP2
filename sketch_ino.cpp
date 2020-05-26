#include <unistd.h>
#include "core_simulation.h"
#include "base_donnees.h"



// la fonction d'initialisation d'arduino
void Board::setup(){
  // on configure la vitesse de la liaison
  Serial.begin(9600);
// on fixe les pin en entree et en sorite en fonction des capteurs/actionneurs mis sur la carte

  pinMode(1,INPUT);
  pinMode(2,INPUT);


}

// la boucle de controle arduino
void Board::loop(){

    char buf[100];
    char buf_Rfid[4]; // nous permet de recueillir la sequence du taf_RFID du badge d'un employeur
    char buf2[100];


    // on demande au detecteur de metaux ce qu'il a detecte et on l'affiche sur le terminal. On suppose en effet
    // qu'on recoit sur notre TERMINAL les objets que portent l'employe comme dans un aeroport
    bus.requestFrom(1,buf,100);
    Serial.println(buf);


    // si on est rentre dans ce if , ca veut dire qu'un employe est sur le point de passer son badge RFID  au lecteur RFID
    if(digitalRead(1) && strcmp( buf, "Boutton ON , aucun objet metallique, veuillez avancer" )==0  ){

            // SCK est initialise a 0 (normal car au debut, aucun employe, donc on ne demande pas au lecteur RFID d'ecrire dans MISO
            // on met donc sck a 1 pour signaler au lecteur rfid qu'a son prochain run il doit communiquer le tag qu'il va detecter,
            // en realite, ce lecteur RFID doit simuler une nouvelle sequence de tag rfid. Ce qui est tres important de comprendre
            // c'est que lorsqu'on met SCK a 1, le lecteur RFID (qui est l'esclave dans la communication MOSI-MISO, et qui ecrit donc MISO)
            // verra que SCK = 1 que lorsque son thread sera active, c'est pour cela que lorsque l'on met SCK a 1 dans le premier if,
            // on peut rien analyser ie on ne peut pas encore lire dans MISO, et pour cela il faut attendre que le lecteur RFID ecrit
            // dans Miso donc quand ce sera le tour du Run() du lecteur RFID, MISO sera remplie et seulement ici on pourra lire dans MISO
            // et si sck etait deja a 1,
            //  cela signifie que deja avant, un premier employe est arrive au lecteur RFID, on est entrain de traiter son passage
            // au lecteur RFID, et seulement au prochain tour on pourra traiter l'employe qui a valide le if ci-dessus,
            // il y a en effet cette notion de "on traite au prochain tour" car on attend que le lecteur RFID ecrit dans MISO,
            // car c'est seulement grace a la sequence envoye par le lecteur RFID via MISO, qu'on peut enfin
            // extraire la sequence du tag RFID de l'employe qui vient de se presenter au lecteur

            if (sck ==0){
                sck =1;
            }
            else if (sck == 1){

                for(int i = 0; i<3;i++){
                        buf_Rfid[i] = miso[i];

                }

                sprintf(buf2,"Server %s",buf_Rfid);
                bus.write(2,buf2,100);


            }
    }
            // si on rentre dans ce else if, cela veut dire que dans le run d'avant une personne arrive
            //au nivau du lecteur, mais que la il n'y a plus personne qui comptE arriver, donc on remet sck a 0 comme ca le
            // lecteur verra (quand son thread s'activera au prochain tour) que personne n'arrive,
            //et qu'il ne doit pas essayer d'ecrire dans MISO

    else if (sck == 1){

                for(int i = 0; i<3;i++){
                        buf_Rfid[i] = miso[i];

                }

                sck = 0;


                sprintf(buf2,"Server %s",buf_Rfid);
                bus.write(2,buf2,100);

            }
            if(digitalRead(2)==1){

                bus.write(3,"Bienvenue a l'INSA",100);

            }
    // 2
    //5
    sleep(5);

  }


// 2 problemes a corriger : la synchronisation des thread --> 2 variables statiques : au capteur et au lecteur rfid
//
