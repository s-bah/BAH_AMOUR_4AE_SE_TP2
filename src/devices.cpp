#include "devices.h"

using namespace std;



Detecteurs :: Detecteurs(): Device(){}


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
                sleep(4);

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
    sleep(5);
    }
}


// ------------------------------------------------ PC Client -------------------------------------




    PC_Client :: PC_Client(): Device(){}
    char * PC_Client :: ask_Serveur(){

        ;
    }

    void PC_Client :: run(){
        SOCKET sock;
        SOCKADDR_IN address;


        long ok;
        char MESSAGE[10];

        WSAData  WSD;
        WORD DllVersion;
        DllVersion = MAKEWORD(2,1);
        ok = WSAStartup(DllVersion, &WSD);

        while(true){

            i2cbus->requestFrom(i2caddr,Buf_Client,10);
            if (strcmp(Buf_Client,"Server")==0){
                cout<<" Echange client server"<<endl;
                cout<<endl;
            }
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
      cout << "---screen :"<< buf << endl;
    }
    // 1
    sleep(3);
    }
}






