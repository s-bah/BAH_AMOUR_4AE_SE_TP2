#ifndef DEVICES_H_INCLUDED
#define DEVICES_H_INCLUDED

#include <iostream>
#include <thread>
#include <unistd.h>
#include <string.h>
#include "core_simulation.h"
#include <vector>
#include <cmath>

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

class Detecteurs_Metaux : public Detecteurs{
    
    private :
        //friend class Buzzer;
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

// ------------------------------------------------ Buzzer -------------------------------------

class Buzzer : public Device{
  private:
    double tempsMin;
    double tempsMax; // Le temps que va sonner le buzzer
    int n; // Le nombre de demi-période

  public:
    // Construteur
    Buzzer();
    vector<double> linspace(double min, double max, int n); // Le vecteur du temps pour generer un signal 50% 50% à 5v et 0v
    void tone();
    virtual void run();
};
// ------------------------------------------------ Servomoteur -------------------------------------
/*class servo : public Device{
  private:
    int angle;
    int etatServo;
    int vitesse;
    int etatDebut;
    int etatFin;
    

  public:
    // Construteur
    servo(int eS);
    void ouvrir();
    void fermer();
    virtual void run();*/

#endif // DEVICES_H_INCLUDED
