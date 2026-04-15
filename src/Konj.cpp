#include<iostream>
#include<vector>
#include<cmath>
#include "Figura.h"
#include "Tabla.h"
#include "Konj.h"

    Konj::Konj(int boja,Tabla* tabla,int slovo,int broj):Figura(slovo,broj,'k',boja){
        this->tabla=tabla;
        
    }

    bool Konj::pomeri(int slovo,int broj){

        int tmp_slovo=poz_slovo;
        int tmp_broj=poz_broj;

        if(pinned || (tabla->polja[slovo][broj]!=NULL && tabla->polja[slovo][broj]->vrati_boju()==boja)){
            return false;
        }
        else if(polja_koja_napada.find(std::make_pair(slovo,broj))!=polja_koja_napada.end()){

            obrisi_polja_koja_napada();
            //Ukoliko jede protivnicku figuru
            if(tabla->polja[slovo][broj]!=NULL){
                Figura* f=tabla->polja[slovo][broj];
                f->obrisi_polja_koja_napada();
                char c=f->vrati_oznaku();

                if(c=='Q' || c=='b' || c=='r'){
                    int b=f->vrati_boju();
                    if(b==1){
                        tabla->moguci_beli_pinneri.erase(f);
                    }
                    else{
                        tabla->moguci_crni_pinneri.erase(f);
                    }
                }
            }
            //U svakom slucaju, menjamo poziciju konja na novu poziciju
            tabla->polja[poz_slovo][poz_broj]=NULL;
            poz_slovo=slovo;
            poz_broj=broj;
            tabla->polja[poz_slovo][poz_broj]=this;

            std::set<std::pair<int,int> > skup_za_iteraciju=tabla->figure_koje_napadaju_polje[tmp_slovo][tmp_broj];
            //Za svaku figuru koja je napadala prehodnu poziciju konja, azuriramo polja koja napada
            for(std::pair<int,int> p : skup_za_iteraciju){
                std::cout<<"Uslo prvo\n"<<p.first<<"  "<<p.second;
                
                tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
            }
            
            
            //Za svaku figuru koja napada novu poziciju konja, azuriramo polja koja napada
            skup_za_iteraciju=tabla->figure_koje_napadaju_polje[poz_slovo][poz_broj];
            for(std::pair<int,int> p : skup_za_iteraciju){
                std::cout<<"\n"<<poz_slovo<<" " << poz_broj<<" Uslo drugo\n"<<p.first<<"  "<<p.second;
                
                
                tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
            }

            odredi_polja_koja_napada();

            return true;
        }
        else return false;
    }



    bool Konj::dozvoljen_En_Passant(){return false;}

    void Konj::onemoguci_En_Passant(){}

    void Konj :: odredi_polja_koja_napada(){

        polja_koja_napada.clear();

        int kandidat_slovo=poz_slovo+1;
        int kandidat_broj=poz_broj+2;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){

            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_broj=poz_broj-2;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_slovo=poz_slovo+2;
        kandidat_broj=poz_broj+1;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_broj=poz_broj-1;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_slovo=poz_slovo-1;
        kandidat_broj=poz_broj+2;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_broj=poz_broj-2;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_slovo=poz_slovo-2;
        kandidat_broj=poz_broj+1;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        kandidat_broj=poz_broj-1;

        if(validna_pozicija(kandidat_slovo,kandidat_broj)){
            polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
            tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
        }

        std::cout<<"Polja koja napada konj"<<poz_slovo<<poz_broj<<"\n";
            for(std::pair< int, int> p : polja_koja_napada){
                std::cout<<p.first<<"  "<<p.second<<"\n";
            }


    }

    void Konj :: obrisi_polja_koja_napada(){
        
        for(std::pair< int, int> p : polja_koja_napada){
            tabla->figure_koje_napadaju_polje[p.first][p.second].erase(std::make_pair(poz_slovo,poz_broj));
        }
        
        polja_koja_napada.clear();
    }

    bool Konj::vidi_kralja(){}
    