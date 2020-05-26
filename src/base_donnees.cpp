
#include "base_donnees.h"

using namespace std;

bool rechercher_base_donnees(char * tag_recherche){
    bool resultat = false;
    cout<<"ON EST DANS LE FICHIER : "<<endl<<endl;
    static int i =0;

    // permettra de stocker la ligne etudiee, en cours,
    string ligne = "initial";

// on ouvre un flux en lecture (input) qu'on appelle flux_fichier
    ifstream flux_fichier("base_donnees.txt");

//on verifie que l'ouverture du flux s'est bien passee

    if (flux_fichier){
            while( getline(flux_fichier, ligne) && resultat== false ){

                cout<<"ligne : "<<ligne<<endl;
                cout<<"tag : "<<tag_recherche<<endl;

                if(strcmp(tag_recherche,ligne.c_str())==0){
                    resultat= true;
                    cout<<"test base de donnees : true"<<endl;
                }
                i++;

            }
    if(resultat==false)
            cout<< "Desole veuillez quitter, vous n'avez pas le droit d'acces"<<endl;
    }
    cout<<endl<<endl;

    return resultat;

}
