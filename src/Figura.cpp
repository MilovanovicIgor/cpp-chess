#include<iostream>

#include "Figura.h"

        
        Figura::Figura(int slovo, int broj, char c, int boja){
            this->poz_slovo=slovo;
            this->poz_broj=broj;
            this->boja=boja;
            this->oznaka=c;
            this->pinned=false;
            this->pinned_by=NULL;
        }

        char Figura :: vrati_oznaku(){
            return this->oznaka;
        }
        int Figura :: vrati_boju(){
            return this->boja;
        }

        void Figura :: pinn(Figura* f){
            this->pinned=true;
            this->pinned_by=f;
            std::cout<<"Pinnovano "<< this->vrati_oznaku()<<" "<< this->poz_slovo<<" "<<poz_broj<<"\n";
        }

        void Figura :: unpinn(){
            this->pinned=false;
            this->pinned_by=NULL;
            std::cout<<"Unpinnovano "<< this->vrati_oznaku()<<" "<< this->poz_slovo<<" "<<poz_broj<<"\n";
        }

    
        bool Figura:: dozvoljen_En_Passant(){
           
        }
        void Figura::onemoguci_En_Passant(){}
        
        bool Figura:: pomeri(int slovo,int broj){

        }

        void Figura :: odredi_polja_koja_napada(){}

        void Figura :: obrisi_polja_koja_napada(){}

       

