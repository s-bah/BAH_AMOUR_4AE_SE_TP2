#include "devices.h"

using namespace std;



Detecteurs :: Detecteurs(): Device(){}

// ------------------------------------------------ classe Interrupteur -------------------------------------


    Interrupteur :: Interrupteur() : state(0){}

    int Interrupteur :: getState() {return state;}
    void Interrupteur :: setState(int i){state = i;}
    void Interrupteur :: run(){
        while(1){

                    *ptrmem = state;

            sleep(3);
        }
    }

// ------------------------------------------------ LED -------------------------------------


LED::LED(int t,string couleur,int num):Device(),state(LOW),couleur_eclairement(couleur),numero(num){
}


void LED::run(){
  while(1){
    if(ptrmem!=NULL)
      state=*ptrmem;
    if (state==LOW)
      cout << "LED numero " << numero <<"((((eteint))))\n";
    else
    cout << "LED numero "<< numero <<" / "<<" couleur : couleur_eclairement / "<<"((((allume))))\n";
    // 1
    sleep(3);
    }
}

//-------------------------------------------------- Capteur_presence----------------------------------------------

Capteur_presence :: Capteur_presence(): Detecteurs(){boutton = "OFF";}

bool Capteur_presence :: detecter(){
    bool resultat;
    if(ifstream("on.txt")){
            boutton = "ON";
            resultat = true;
        }
        else{
            boutton = "OFF";
            resultat = false;
        }
    return resultat;
}

char * Capteur_presence :: getBoutton(){return boutton;}

void Capteur_presence :: run(){


    while(1){
     if (detecter()){
        *ptrmem=1;
     }
     else{
        *ptrmem=0;
     }
     //3
    sleep(5);
    }
}

// ------------------------------------------------ Detecteurs_Metaux -------------------------------------

Detecteurs_Metaux :: Detecteurs_Metaux() : Detecteurs(){}

Capteur_presence & Detecteurs_Metaux :: getCapteur(){

    return (*this).capteur_presence;
}

void Detecteurs_Metaux :: sonner(){



    cout<<"ALERTE ALERTE ALERTE , OBJETS METALLIQUES DETECTES  "<<endl<<endl;


    }



bool Detecteurs_Metaux :: detecter(){

        bool resultat = false;
        int i = 0;

        while ( resultat==false && (i<4)){
            if (liste_metaux[i] == objet_porte){
            resultat = true;
            }
        i++;
        }

        return resultat;

}

void Detecteurs_Metaux :: run(){
    char buf[100];
    int i =0;

    while(1){

        objet_porte = (char) (97+i);
        i++;

        if(capteur_presence.detecter() ){

            if(detecter()){
                sprintf(buf,"Boutton %s , objet metallique porte : %c ",capteur_presence.getBoutton(),objet_porte);

                i2cbus->write(i2caddr,buf,100);
                //1
                //3
                //6
                sleep(7);
                sonner();
                }
            else{
                 sprintf(buf,"Boutton %s , aucun objet metallique, veuillez avancer",capteur_presence.getBoutton());
                i2cbus->write(i2caddr,buf,100);
                // 2
                //4
                sleep(6);

                }

            }

        else{
            sprintf(buf,"aucun employe detecte");
            i2cbus->write(i2caddr,buf,100);
        }

    if(i==11){
        i=0;
    }
    // 3
    //5
    sleep(6);
    }
}


// ------------------------------------------------ PC Client -------------------------------------




    PC_Client :: PC_Client(): Device(){}

    void PC_Client :: construire_Adresse_Serveur(){
                    address.sin_addr.s_addr = inet_addr(addr_IP_Server);
                    address.sin_family = AF_INET;
                    address.sin_port = htons(10103);

    }

    bool PC_Client :: ask_Serveur(char * tag){
                    bool resultat;
                    // si on est rentre dans cette fonction, tag est forcement non NULL
                    char tab_tag [10]; // On stocke la sequence du tag en cours dans un tableau
                    char * msg_send;
                    msg_send = strcpy(tab_tag,tag);
                    sock = socket(AF_INET,SOCK_STREAM,NULL);

                   construire_Adresse_Serveur();

                if(connect(sock, (SOCKADDR *) &address, sizeof(address))<0){

                    perror("connect()");
                    fprintf(stderr,"%s\n",strerror(errno));
                }

                int size_envoi = sizeof(tab_tag);

                while(size_envoi>0){
                    int bytes_sent = send(sock,msg_send,size_envoi,0);
                    if(bytes_sent < 0){
                        perror("send()");
                        fprintf(stderr,"%s\n",strerror(errno));
                    }
                    size_envoi -= bytes_sent;
                    msg_send +=bytes_sent;

                }


                // apres avoir envoyer la requete d'identification du tag_RFID au serveur, on se met en mode
                // " on attend la reponse du serveur

                char msg_recu [10];
                char * ptr_recv = msg_recu;
                int size_reception = sizeof(msg_recu);

                while(size_reception > 0){

                    int  bytes_received = recv(sock,ptr_recv,size_reception,0);

                      if(bytes_received < 0){
                        perror("recv()");
                        fprintf(stderr,"%s\n",strerror(errno));
                      }
                      size_reception -= bytes_received;
                      ptr_recv += bytes_received;
                }

                // si msg_recu = true, cela veut dire que le tag_RFID envoye au serveur fait parti
                // des tags autorises
                if( strcmp(msg_recu,"true") == 0 ){
                    resultat = true;

                }

    }



    // on veut juste recuperer la sequence du tag pour l'envoyer au serveur, comme si on envoyait un echantillon
    // au laboratoire

    char * PC_Client :: analyser_Tag(char * message){

        char * resultat = message;

    if (message != NULL){

        while ((*resultat) != ' '){
            resultat++;
        }
        resultat++;

    }

    return resultat;

    }

    Interrupteur& PC_Client :: getInterrupteur(){
        return (*this).interrupteur;
    }


    void PC_Client :: run(){

        long ok;

        // Cette manip est necessaire pour prevenir le systeme qu'on va utiliser un socket
        WSAData  WSD;
        WORD DllVersion;
        DllVersion = MAKEWORD(2,1);
        ok = WSAStartup(DllVersion, &WSD);

        while(true){
            interrupteur.setState(0);
            i2cbus->requestFrom(i2caddr,Buf_Client,100);

            if (strstr(Buf_Client,"Server")!=NULL) // On regarde si la chaine "Server" est contenu dans le message envoye
                                                    // par l'arduino, dans quel cas, cela signifie que l'arduino
                                                    // cherche a faire analyser un tag, detecte par le lecteur RFID
                {
                char * tag = analyser_Tag(Buf_Client); // on extrait le tag du message envoye par l'arduino, en effet
                                                        // on rappelle que lorsque l'arduino veut faire analyser un
                                                        // tag RFID, l'arduino doit preciser le message "Serveur sequence"

                if (tag!=NULL){


                   if( ask_Serveur(tag) == true)//on se connecte au serveur depuis l'instance
                                                //client appartenant  a la classe PC_Client
                                                // si le tag analyse appartient a la base de donnees
                                                // on valide le passage de l'employeur
                   {
                       interrupteur.setState(1);
                       cout<<"serveur says tag " <<tag<<" exists"<<endl;

                   }


                }// TAG != NULL


            }// demande pour le serveur recu

        sleep(3);
        }

    }




// ------------------------------------------------ SCREEN -------------------------------------

Screen::Screen ():Device(){
  }

void Screen::run(){
  while(1){
    if ( (i2cbus!=NULL)&&!(i2cbus->isEmptyRegister(i2caddr))){
      Device::i2cbus->requestFrom(i2caddr, buf, I2C_BUFFER_SIZE);
      cout << "---screen : "<< buf << endl<<endl<<endl;
    }
    // 1
    sleep(4);
    }
}






