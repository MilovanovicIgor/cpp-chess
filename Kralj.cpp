#include<iostream>
#include<vector>

#include "Tabla.h"
#include "Figura.h"
#include "Top.h"
#include "Kralj.h"
#include "Lovac.h"
#include "Kraljica.h"

Kralj::Kralj(int boja,Tabla* tabla,int slovo,int broj):Figura(slovo,broj,'K',boja){
    this->tabla=tabla;
    this->pomeren=false;
}

bool Kralj::pomeri(int slovo,int broj){
    int tmp_slovo=poz_slovo;
    int tmp_broj=poz_broj;

    if(tabla->napadnuto_polje(slovo,broj,boja)){
        return false;
    }
            
    if(polja_koja_napada.find(std::make_pair(slovo,broj))!=polja_koja_napada.end()){
            
        if((tabla->polja[slovo][broj]==NULL || tabla->polja[slovo][broj]->vrati_boju()!=this->vrati_boju())){
                    
            
            if(tabla->polja[slovo][broj]!=NULL){

                Figura* f=tabla->polja[slovo][broj];

                if(tabla->napadnuto_polje(slovo,broj,boja)){
                    return false;
                }

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

            obrisi_polja_koja_napada();

            tabla->polja[poz_slovo][poz_broj]=NULL;
            poz_slovo=slovo;
            poz_broj=broj;
            tabla->polja[poz_slovo][poz_broj]=this;
            pomeren=true;

            std::set<std::pair<int,int> > skup_za_iteraciju=tabla->figure_koje_napadaju_polje[tmp_slovo][tmp_broj];

            for(std::pair<int,int> p : skup_za_iteraciju){

                tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();    
                tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
            }
            
            skup_za_iteraciju=tabla->figure_koje_napadaju_polje[poz_slovo][poz_broj];

            for(std::pair<int,int> p : skup_za_iteraciju){
                
                tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
            }
                    

            odredi_polja_koja_napada();

            if(boja==1){
                tabla->Kralj_beli=std::make_pair(poz_slovo, poz_broj);
                
            }
            else{
                tabla->Kralj_crni=std::make_pair(poz_slovo,poz_broj);
            }

            return true;
         }
         else{
            return false;
        }

    }
    else if(!pomeren && broj==poz_broj){
        
        if(slovo==6 && tabla->polja[7][broj]->vrati_oznaku()=='r'){
            if(!tabla->napadnuto_polje(5,broj,boja) && !tabla->napadnuto_polje(6,broj,boja) && !tabla->napadnuto_polje(4,broj,boja)){

                Top* t=dynamic_cast<Top*>(tabla->polja[7][broj]);
                if(!(t->getPomeren()) && tabla->polja[5][broj]==NULL && tabla->polja[6][broj]==NULL){

                    t->obrisi_polja_koja_napada();
                    obrisi_polja_koja_napada();

                    tabla->polja[7][broj]=NULL;
                    tabla->polja[poz_slovo][poz_broj]=NULL;

                    poz_slovo=slovo;
                    tabla->polja[6][broj]=this;
                    
                    t->setPozSlovo(5);
                    tabla->polja[5][broj]=t;

                    odredi_polja_koja_napada();
                    t->odredi_polja_koja_napada();
                    std::set<std::pair<int,int> > skup_za_iteraciju;
                    for(int i=4;i<8;i++){
                        skup_za_iteraciju=tabla->figure_koje_napadaju_polje[i][broj];
                        for(std::pair<int,int> p : skup_za_iteraciju){
                            tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                        }
                    }
                    pomeren=true;

                    if(boja==1){
                        tabla->Kralj_beli=std::make_pair(poz_slovo, poz_broj);
                    }
                    else{
                        tabla->Kralj_crni=std::make_pair(poz_slovo,poz_broj);
                    }

                    return true;

                }

            }
        }
        else if(slovo==2 && tabla->polja[0][broj]->vrati_oznaku()=='r'){

            if(!tabla->napadnuto_polje(2,broj,boja) && !tabla->napadnuto_polje(3,broj,boja) && !tabla->napadnuto_polje(4,broj,boja)){

                Top* t=dynamic_cast<Top*>(tabla->polja[0][broj]);
                if(!(t->getPomeren()) && tabla->polja[1][broj]==NULL && tabla->polja[2][broj]==NULL && tabla->polja[3][broj]==NULL){

                    t->obrisi_polja_koja_napada();
                    obrisi_polja_koja_napada();

                    tabla->polja[0][broj]=NULL;
                    tabla->polja[poz_slovo][poz_broj]=NULL;

                    poz_slovo=slovo;
                    tabla->polja[2][broj]=this;
                        
                    t->setPozSlovo(3);
                    tabla->polja[3][broj]=t;

                    odredi_polja_koja_napada();
                    t->odredi_polja_koja_napada();
                    
                    std::set<std::pair<int,int> > skup_za_iteraciju;
                    for(int i=0;i<5;i++){
                        skup_za_iteraciju=tabla->figure_koje_napadaju_polje[i][broj];
                        for(std::pair<int,int> p : skup_za_iteraciju){
                            tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                        }
                    }
                    pomeren=true;

                    if(boja==1){
                        tabla->Kralj_beli=std::make_pair(poz_slovo, poz_broj);
                        }
                    else{
                        tabla->Kralj_crni=std::make_pair(poz_slovo,poz_broj);
                    }

                    return true;

                }
            }
        }

    }
    else{
            
        return false;
    }
}
bool Kralj::dozvoljen_En_Passant(){
    return false;
    }

void Kralj :: onemoguci_En_Passant(){}

bool Kralj:: vidi_kralja(){
    return false;
}

int Kralj::vrati_poziciju_slovo(){
    return poz_slovo;
}

int Kralj::vrati_poziciju_broj(){
    return poz_broj;
}

void Kralj :: odredi_polja_koja_napada(){
    polja_koja_napada.clear();

    int kandidat_slovo=poz_slovo;
    int kandidat_broj=poz_broj+1;

    if(validna_pozicija(kandidat_slovo,kandidat_broj)){

        polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
        tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
    }

    kandidat_broj=poz_broj-1;

    if(validna_pozicija(kandidat_slovo,kandidat_broj)){
        polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
        tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
    }

    kandidat_slovo=poz_slovo+1;
    kandidat_broj=poz_broj;

    if(validna_pozicija(kandidat_slovo,kandidat_broj)){
        polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
        tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
    }

    kandidat_slovo=poz_slovo-1;

    if(validna_pozicija(kandidat_slovo,kandidat_broj)){
        polja_koja_napada.insert(std::make_pair(kandidat_slovo,kandidat_broj));
        tabla->figure_koje_napadaju_polje[kandidat_slovo][kandidat_broj].insert( std::make_pair(poz_slovo , poz_broj));
    }

    kandidat_slovo=poz_slovo+1;
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

    std::cout<<"Polja koja napada kralj"<<poz_slovo<<poz_broj<<"\n";
    for(std::pair< int, int> p : polja_koja_napada){
        std::cout<<p.first<<"  "<<p.second<<"\n";
    }

}

void Kralj :: obrisi_polja_koja_napada(){
    for(std::pair< int, int> p : polja_koja_napada){
        tabla->figure_koje_napadaju_polje[p.first][p.second].erase(std::make_pair(poz_slovo,poz_broj));
    }
        
    polja_koja_napada.clear();
}

bool Kralj :: moze_se_pomeriti(){
    std::cout<<"Uslo u proveru moze_se_pomeriti\n";
    
    

    for(std::pair<int,int> p : polja_koja_napada){
        std::cout<<p.first<<" "<<p.second<<"\n";
        Figura* f=tabla->polja[p.first][p.second];
        if(f==NULL){
            if(!tabla->napadnuto_polje(p.first,p.second,boja)){
                return true;
            }
        }
        else if(f->vrati_boju()==boja){
            //Na ovom polju je figura kraljeve boje
            //continue pretpostavljam treba
        }
        else{//Ako je figura protivnicka, kralj moze da se pomeri i jede je, ukoliko ta figura nije branjena drugom protivnickom figurom
            if(!tabla->napadnuto_polje(p.first,p.second,boja)){
                return true;
            }
        }
    }
    std::cout<<"zavrseno\n";
    return false;

}