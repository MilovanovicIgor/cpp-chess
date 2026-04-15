#include<iostream>
#include<vector>
#include<stdlib.h>
#include<cmath>
#include<string>
#include "Figura.h"

#include "Tabla.h"

#include "Pijun.h"
#include "Igra.h"
class Konj : public Figura{


};

class Lovac : public Figura{


};

class Top : public Figura{

};

class Kralj : public Figura{

};

class Kraljica : public Figura{
};



/*void igraj(Tabla tabla){
    tabla.ispisi_tablu();
    int i=1;
    while(true){
            if(i==1){
                std::cout<<"Beli";
            }
            else if(i==2){
                std::cout<<"Crni";
            }
            else{
                std::cout<<"Losa boja";
                exit(EXIT_FAILURE);
            }
            std::cout<<" igrac na potezu\n";
            std::string s1,s2;
            std::cin>>s1>>s2;
            char oznaka=s1[0];
            int poz_slovo=s1[1]-'a';
            int poz_broj=s1[2]-'1';

            int slovo=s2[0]-'a';
            int broj=s2[1]-'1';

            

            if(tabla.polja[poz_slovo][poz_broj]!=NULL){
                Figura *f=tabla.polja[poz_slovo][poz_broj];
                

                if(f->vrati_boju()!=i){
                    std::cout<<"Ne mozete pomeriti figuru protivnika\n";
                    tabla.ispisi_tablu();
                }
                bool proslo=f->pomeri(slovo,broj);
                if(!proslo){
                    std::cout<<"Nevalidan potez\n";
                    tabla.ispisi_tablu();
                }
                else{
                    i=3-i;
                    tabla.ispisi_tablu();
                }
            }
            else{
                std::cout<<"Prazno polje\n";
            }

        
    }
}*/





int main(){
    std::cout<<"Pokrenut\n";

    Tabla tabla=Tabla();

    tabla.postavi_pocetnu_poziciju();
    
    Igra igra=Igra(&tabla);
    igra.pocni_igru();

    
    return 0;
}