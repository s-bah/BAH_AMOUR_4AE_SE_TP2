
#include "mydevices.h"

using namespace std;

int luminosite_environnement = 200;

//classe AnalogSensorTemperature
AnalogSensorTemperature::AnalogSensorTemperature(int d,int  t):Device(),val(t),temps(d){
  alea=1;
}

void AnalogSensorTemperature::run(){
  while(1){
    alea=1-alea;
    if(ptrmem!=NULL)
      *ptrmem=val+alea;
    sleep(temps);
  }
}


//--------------------------------------------- AnalogSensorLuminosity ----------------------------------------------------------


AnalogSensorLuminosity :: AnalogSensorLuminosity(int t):Device(),temps(t){
    alea = 0;
    }

void AnalogSensorLuminosity :: run() {
   int i = 1;
    while(1){
            if (i>=3){
                alea = 0;
                i=0;
            }
            else {
               i++;
               alea += 10;
            }

        if(ptrmem!=NULL)
        *ptrmem = luminosite_environnement+ alea;
      sleep(temps);
    }
}



//--------------------------------------------- DigitalActuatorLED -------------------------------------------------------------

//classe DigitalActuatorLED
DigitalActuatorLED::DigitalActuatorLED(int t):Device(),state(LOW),temps(t){
}

void DigitalActuatorLED::run(){
  while(1){
    if(ptrmem!=NULL)
      state=*ptrmem;
    if (state==LOW)
      cout << "((((eteint))))\n";
    else
    cout << "((((allume))))\n";
    sleep(temps);
    }
}

 //--------------------------------------------- IntelligentDigitalActuatorLED -------------------------------------------------------------
IntelligentDigitalActuatorLED :: IntelligentDigitalActuatorLED(int t):  DigitalActuatorLED(t){}
void IntelligentDigitalActuatorLED :: run(){
    while(1){
        if(ptrmem!=NULL)
          state=*ptrmem;
        if (state==LOW){
          cout << "((((eteint))))\n";
          luminosite_environnement = luminosite_environnement -50 ;
        }
        else{
        cout << "((((allume))))\n";
        luminosite_environnement += 50;

        }
         sleep(temps);
        }
}

//--------------------------------------------- I2CActuatorScreen -------------------------------------------------------------

// classe I2CActuatorScreen
I2CActuatorScreen::I2CActuatorScreen ():Device(){
  }

void I2CActuatorScreen::run(){
  while(1){
    if ( (i2cbus!=NULL)&&!(i2cbus->isEmptyRegister(i2caddr))){
      Device::i2cbus->requestFrom(i2caddr, buf, I2C_BUFFER_SIZE);
      cout << "---screen :"<< buf << endl;
    }
    sleep(1);
    }
}

//--------------------------------------------- I2CActuatorScreen -------------------------------------------------------------

// classe ExternalDigitalSensorButton

// par defaut on met le bouton a faux : c'est un choix d'initialisation
ExternalDigitalSensorButton :: ExternalDigitalSensorButton(): Device(), boutton(OFF){}


void ExternalDigitalSensorButton :: run(){
    while(1){
        if(ifstream("on.txt")){
            boutton = ON;
            *ptrmem = 1;
        }
        else{
            boutton = OFF;
            *ptrmem = 0;

        }
    }
}





