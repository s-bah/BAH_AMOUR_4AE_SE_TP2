#ifndef DEVICES_H_INCLUDED
#define DEVICES_H_INCLUDED

    #include <iostream>
    #include <thread>
    #include <unistd.h>
    #include <string.h>
    #include "core_simulation.h"
    #include <vector>


// pour utiliser la programmation de sockets


    #include <conio.h>
    #include <stdio.h>

    #include <WinSock2.h>
    #include <Windows.h>
    #include <sstream>
    #include <string>


    #define SCK_VERSION 0x0202





    #define taille_objet 10



class Detecteurs : public Device {

public :
    Detecteurs();
    virtual bool detecter()=0; // methode abstraite, car la methode depend de la classe fille



};

// ------------------------------------------------ LED -------------------------------------

class LED: public Device {
protected:

  int state;
  int numero;
  string couleur_eclairement;

public:
    // initialisation du temps de rafraichiisement
    LED(int t, string couleur, int num);

  // thread representant l'actionneur et permettant de fonctionner independamment de la board
  virtual void run();
};

//-------------------------------------------------- Capteurs ----------------------------------------------

class Capteur_presence : public Detecteurs{
protected :
    char * boutton;

public :

    Capteur_presence();
    bool detecter();
   char * getBoutton();
    void run();

};


//-------------------------------------------------- Detecteur_Metaux ----------------------------------------------


// il est tres important de comprendre physique comment nous imaginons le systeme,
// on suppose que le capteur de presense est integre dans la porte detecteur de metaux
class Detecteurs_Metaux : public Detecteurs{
    protected :
        char objet_porte ;
        char  liste_metaux[4] = {'a','d','h','j'};
        Capteur_presence capteur_presence; // pour detecter si une personne arrive devant la porte detecteur de metaux




    public :
        Detecteurs_Metaux();
        Capteur_presence & getCapteur();

        void sonner();
        bool detecter();
        virtual void run();

};


// ------------------------------------------------ interrupteur ------------------------------------------------------


class Interrupteur : public Device{

protected :
    int state ;
public :
    Interrupteur();
    int getState();
    void setState(int i);
    virtual void run();

};

// ------------------------------------------------ PC Client -------------------------------------

class PC_Client : public Device{

protected :
    char addr_IP_Server[10] = "127.0.0.1";
    char Buf_Client[100];
    SOCKET sock;
    SOCKADDR_IN address;
    Interrupteur interrupteur;


public :
    PC_Client();
    void construire_Adresse_Serveur();
    char * analyser_Tag(char * message); // analyse une sequence RFID contenue dans le badge d'un employe
    bool ask_Serveur(char * tag); // se connecte au serveur distant et demande au serveur si cette sequence existe

    Interrupteur& getInterrupteur();

    virtual void run();

};


// ------------------------------------------------ SCREEN -------------------------------------

class Screen : public Device{
protected:
    // memorise l'affichage de l'ecran
  char buf[I2C_BUFFER_SIZE];

public:
  // constructeur
 Screen ();
  // thread representant le capteur et permettant de fonctionner independamment de la board
  virtual void run();
};


#endif // DEVICES_H_INCLUDED
