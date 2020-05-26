#include "devices.h"

//#include <iostream>

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
    sleep(3); 
    }
}

// ------------------------------------------------ Detecteurs_Metaux -------------------------------------

Detecteurs_Metaux :: Detecteurs_Metaux() : Detecteurs(){}

Capteur_presence & Detecteurs_Metaux :: getCapteur(){

    return (*this).capteur_presence;
}

void Detecteurs_Metaux :: sonner(){

    //Faire sonner un buzzer

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
                sprintf(buf,"Boutton %s , objet metallique porte  ",capteur_presence.getBoutton());

                i2cbus->write(i2caddr,buf,100);
                //1
                sleep(3);
                sonner();
                }
            else{
                 sprintf(buf,"Boutton %s , aucun objet metallique, veuillez avancer svp",capteur_presence.getBoutton());
                i2cbus->write(i2caddr,buf,100);
                // 2
                sleep(4);

                }

            }

        else{
            sprintf(buf,"aucun employe detecte \n");
            i2cbus->write(i2caddr,buf,100);
        }

    if(i==11){
        i=0;
    }
    // 3
    sleep(5);
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

// ------------------------------------------------ BUZZER -------------------------------------

Buzzer :: Buzzer():Device(){
    double tempsMin = 0;
    double tempsMax = 5; // Le temps que va sonner le buzzer (5 s)
    int n; // Le nombre de demi-période
    }

vector<double> Buzzer :: linspace(double tempsMin, double tempsMax, int n){
	vector<double> result;
	// vector iterator
	int iterator = 0;

	for (int i = 0; i <= n-2; i++)
	{
		double temp = tempsMin + i*(tempsMax-tempsMin)/(floor((double)n) - 1);
		result.insert(result.begin() + iterator, temp);
		iterator += 1;
	}

	//iterator += 1;

	result.insert(result.begin() + iterator, tempsMax);
	return result;
  }

//}


void Buzzer:: tone() {

int const periode(10); // On fixe la frequence de vibration du buzzer à 1Hz par souci de lisibilité à l'affichage sinon, elle est de l'odre de kHz
vector <double> vecteur;
vecteur = linspace(tempsMin, tempsMax, periode); // On étale le temps, ici on considère une frequence de 1Hz donc on aura 2 demi-periode dans chaque période

short int ton[periode];
cout<< "TONALITE ALERTE"<< endl;
for (int i=0; i<periode; i++){
    if (i%2) {
    ton[i]=5;
    }
    else ton[i]=0;
    cout<< ton[i];
 }
}

void Buzzer:: run() {
  while(1){
    if(ptrmem!=NULL) 
  {
    if (*ptrmem==1)   /// Est-ce que l'arduino m'a autorisé à sonner
    {
      tone();
    }
  }
  sleep(3); 
 }
}

// ------------------------------------------------ SERVOMOTEUR -------------------------------------


