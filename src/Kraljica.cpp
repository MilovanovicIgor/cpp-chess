#include<iostream>
#include<cmath>
#include<vector>
#include "Figura.h"
#include "Tabla.h"
#include "Kraljica.h"

    Kraljica::Kraljica(int boja,Tabla* tabla,int slovo,int broj):Figura(slovo,broj,'Q',boja){
        this->tabla=tabla;
        this->pinning=NULL;
    }

    bool Kraljica::pomeri(int slovo,int broj){
        int tmp_slovo=poz_slovo;
        int tmp_broj=poz_broj;

            
        if(polja_koja_napada.find(std::make_pair(slovo,broj))!=polja_koja_napada.end()){
            std::cout<<"Usao";
            
            if((!pinned || jede_pinera(slovo,broj) || ide_prema_pineru(slovo,broj)) && (tabla->polja[slovo][broj]==NULL || tabla->polja[slovo][broj]->vrati_boju()!=this->vrati_boju())){
                    
                obrisi_polja_koja_napada();
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

                tabla->polja[poz_slovo][poz_broj]=NULL;
                poz_slovo=slovo;
                poz_broj=broj;
                tabla->polja[poz_slovo][poz_broj]=this;

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
                return true;
             }
             else{
                return false;
            }

        }
        else{
            
            return false;
        }
    } 
        
        bool Kraljica::dozvoljen_En_Passant() { return false; }

        void Kraljica::onemoguci_En_Passant(){}

        bool Kraljica::jedi(int slovo,int broj){
            if(tabla->polja[slovo][broj]->vrati_boju()==boja){
                return false;
            }
            else if(!pinned || (pinned && jede_pinera(slovo,broj))){
                tabla->polja[poz_slovo][poz_broj]=NULL;
                poz_slovo=slovo;
                poz_broj=broj;
                tabla->polja[poz_slovo][poz_broj]=this;
                return true;
            }
            else
                return false;
        }
        bool Kraljica::jede_pinera(int slovo,int broj){
            if(pinned_by==NULL){
                return false;
            }
            int slovo_pinnera=pinned_by->poz_slovo;
            int broj_pinnera=pinned_by->poz_broj;

            return (slovo==slovo_pinnera && broj==broj_pinnera);
            
        }

        bool Kraljica :: ide_prema_pineru(int slovo, int broj){
            if(pinned_by==NULL){
                return false;
            }
            char oznaka_pinnera=pinned_by->vrati_oznaku();
            int poz_slovo_pinnera=pinned_by->poz_slovo;
            int poz_broj_pinnera=pinned_by->poz_broj;

            int poz_kralja_slovo,poz_kralja_broj;

            if(boja==1){
                poz_kralja_slovo=tabla->Kralj_beli.first;
                poz_kralja_broj=tabla->Kralj_beli.second;
            }
            else{
                poz_kralja_slovo=tabla->Kralj_crni.first;
                poz_kralja_broj=tabla->Kralj_crni.second;
            }

            if(oznaka_pinnera=='b' || (oznaka_pinnera=='Q' && (poz_slovo_pinnera!=poz_slovo && poz_broj_pinnera!=poz_broj_pinnera))){
                int razlika_slovo=slovo-poz_kralja_slovo;
                int razlika_broj=broj-poz_kralja_broj;
                if(std::abs(razlika_slovo)!=std::abs(razlika_broj)){
                    return false;
                }
                if(poz_slovo_pinnera>poz_kralja_slovo){
                    if(poz_broj_pinnera>poz_kralja_broj){
                        if(razlika_slovo>0 && razlika_broj>0 && razlika_slovo<poz_slovo_pinnera-poz_kralja_slovo){
                            return true;
                        }
                            
                    }
                    else if(poz_broj_pinnera<poz_kralja_broj){
                        if(razlika_slovo>0 && razlika_broj<0 && razlika_slovo<poz_slovo_pinnera-poz_kralja_slovo){
                            return true;
                        }
                        
                    }
                    else {
                        return false;
                    }

                }
                else{
                    if(poz_broj_pinnera>poz_kralja_broj){
                        if(razlika_slovo<0 && razlika_broj>0 && razlika_broj<poz_broj_pinnera-poz_kralja_broj){
                            return true;
                        }
                        else {
                            return false;
                        }
                            
                   }
                    else if(poz_broj_pinnera<poz_kralja_broj){
                        if(razlika_slovo<0 && razlika_broj<0 && razlika_slovo>poz_slovo_pinnera-poz_kralja_slovo){
                            return true;
                        }
                        
                  }
                    else {
                        return false;
                    }
                } 
                    
        
            }
            else if(poz_slovo==poz_slovo_pinnera){
                
                return (broj>std::min(poz_kralja_broj,poz_broj_pinnera) && broj<std::max(poz_kralja_broj,poz_broj_pinnera));
            }
            else{
                
                return (slovo>std::min(poz_kralja_slovo,poz_slovo_pinnera) && slovo<std::max(poz_kralja_slovo,poz_slovo_pinnera));
            }
            
        }
        

        bool Kraljica:: vidi_kralja(){
            int poz_kralja_slovo;
            int poz_kralja_broj;
            
            if(boja==1){
                poz_kralja_slovo=tabla->Kralj_crni.first;
                poz_kralja_broj=tabla->Kralj_crni.second;
            }
            else{
                poz_kralja_slovo=tabla->Kralj_beli.first;
                poz_kralja_broj=tabla->Kralj_beli.second;
            }

            if(poz_slovo==poz_kralja_slovo || poz_broj==poz_kralja_broj){
                return true;
            }
            else{
                int razlika_slovo=std::abs(poz_slovo-poz_kralja_slovo);
                int razlika_broj=std::abs(poz_broj-poz_kralja_broj);

                return razlika_slovo==razlika_broj;
            }
            
        }

        void Kraljica :: pinnuj_ako_treba(){
            int poz_slovo_kralja,poz_broj_kralja;

            if(boja==1){
                poz_slovo_kralja=tabla->Kralj_crni.first;
                poz_broj_kralja=tabla->Kralj_crni.second;
            }
            else{
                poz_slovo_kralja=tabla->Kralj_beli.first;
                poz_broj_kralja=tabla->Kralj_beli.second;
            }

            Figura* kandidat=NULL;

            if(poz_slovo>poz_slovo_kralja && poz_broj > poz_broj_kralja){
                
                for(int i=poz_slovo_kralja+1,j=poz_broj_kralja+1;i<poz_slovo && j< poz_broj; i++,j++){
                    Figura* f=tabla->polja[i][j];
                    if(f!=NULL){
                        if(f->boja==boja){
                            return;
                        }
                        else if(kandidat!=NULL){
                            return;
                        }
                        else{
                            kandidat=f;
                        }
                    }
                }

            }
            else if(poz_slovo<poz_slovo_kralja && poz_broj<poz_broj_kralja){
                
                for(int i=poz_slovo+1,j=poz_broj+1;i<poz_slovo_kralja && j< poz_broj_kralja; i++,j++){
                    Figura* f=tabla->polja[i][j];
                        if(f!=NULL){
                            if(f->boja==boja){
                                return;
                            }
                            else if(kandidat!=NULL){
                                return;
                            }
                            else{
                                kandidat=f;
                            }
                        }
                }
            }
            else if(poz_slovo>poz_slovo_kralja && poz_broj<poz_broj_kralja){

                for(int i=poz_slovo-1,j=poz_broj+1;i>poz_slovo_kralja && j<poz_broj_kralja;i--,j++){

                    Figura* f=tabla->polja[i][j];
                    if(f!=NULL){
                        if(f->boja==boja){
                            return;
                        }
                        else if(kandidat!=NULL){
                            return;
                        }
                        else{
                            kandidat=f;
                        }
                    }
                }
            }
            else if(poz_slovo<poz_slovo_kralja && poz_broj>poz_broj_kralja){

                for(int i=poz_slovo+1,j=poz_broj-1;i<poz_slovo_kralja && j>poz_broj_kralja;i++,j--){

                    Figura* f=tabla->polja[i][j];
                    if(f!=NULL){
                        if(f->boja==boja){
                            return;
                        }
                        else if(kandidat!=NULL){
                            return;
                        }
                        else{
                            kandidat=f;
                        }
                    }
                }
            }
            else if(poz_slovo_kralja==poz_slovo){
                int bottom=std::min(poz_broj_kralja,poz_broj);
                int top=std::max(poz_broj_kralja,poz_broj);
                for(int i=bottom+1;i<top;i++){
                    Figura * f=tabla->polja[poz_slovo][i];
                    if(f!=NULL){
                        if(f->boja==boja){
                            return;
                        }
                        else if(kandidat!=NULL){
                            return;
                        }
                        else{
                            kandidat=f;
                        }
                    }
                }
            }
            else{
                int left=std::min(poz_slovo_kralja,poz_slovo);
                int right=std::max(poz_slovo_kralja,poz_slovo);

                for(int i=left+1;i<right;i++){
                    Figura* f=tabla->polja[i][poz_broj];
                    if(f!=NULL){
                        if(f->boja==boja){
                            return;
                        }
                        else if(kandidat!=NULL){
                            return;
                        }
                        else{
                            kandidat=f;
                        }
                    }
                }
            }

            if(kandidat!=NULL){
                kandidat->pinn(this);
                this->pinning=kandidat;
            }
        }

        void Kraljica :: odredi_polja_koja_napada(){
            polja_koja_napada.clear();

            for(int i=poz_broj+1;i<8;i++){

                polja_koja_napada.insert(std::make_pair(poz_slovo,i));
                tabla->figure_koje_napadaju_polje[poz_slovo][i].insert(std::make_pair( poz_slovo, poz_broj));

                if(tabla->polja[poz_slovo][i]!=NULL){
                    break;
                }
            }
            for(int i=poz_broj-1;i>=0;i--){

                tabla->figure_koje_napadaju_polje[poz_slovo][i].insert(std::make_pair( poz_slovo, poz_broj));
                polja_koja_napada.insert(std::make_pair( poz_slovo,i));

                if(tabla->polja[poz_slovo][i]!=NULL){
                    break;
                }
            }
            for(int i=poz_slovo+1;i<8;i++){

                tabla->figure_koje_napadaju_polje[i][poz_broj].insert(std::make_pair( poz_slovo, poz_broj));
                polja_koja_napada.insert(std::make_pair( i,poz_broj));

                if(tabla->polja[i][poz_broj]!=NULL){
                    break;
                }
            }
            for(int i=poz_slovo-1;i>=0;i--){

                tabla->figure_koje_napadaju_polje[i][poz_broj].insert(std::make_pair( poz_slovo, poz_broj));
                polja_koja_napada.insert(std::make_pair(i , poz_broj));

                if(tabla->polja[i][poz_broj]!=NULL){
                    break;
                }
            }
        
        for(int i=poz_slovo+1, j=poz_broj+1;i<8 && j<8;i++,j++){
            std::cout<<"\n Sad";
            polja_koja_napada.insert(std::make_pair(i,j));
            tabla->figure_koje_napadaju_polje[i][j].insert(std::make_pair(poz_slovo,poz_broj));

            if(tabla->polja[i][j]!=NULL){
                break;
            }
        }
        for(int i=poz_slovo+1, j=poz_broj-1;i<8 &&j>=0;i++,j--){

            polja_koja_napada.insert(std::make_pair(i,j));
            tabla->figure_koje_napadaju_polje[i][j].insert(std::make_pair(poz_slovo,poz_broj));

            if(tabla->polja[i][j]!=NULL){
                break;
            }
        }
        for(int i=poz_slovo-1, j=poz_broj+1;i>=0 && j<8;i--,j++){

            polja_koja_napada.insert(std::make_pair(i,j));
            tabla->figure_koje_napadaju_polje[i][j].insert(std::make_pair(poz_slovo,poz_broj));

            if(tabla->polja[i][j]!=NULL){
                break;
            }
        }
        for(int i=poz_slovo-1, j=poz_broj-1;i>=0 && j>=0;i--,j--){

            polja_koja_napada.insert(std::make_pair(i,j));
            tabla->figure_koje_napadaju_polje[i][j].insert(std::make_pair(poz_slovo,poz_broj));
           
            if(tabla->polja[i][j]!=NULL){
                break;
            }
        }

        if(vidi_kralja()){
            std::cout<<this->vrati_oznaku()<<" "<< poz_slovo<< " "<<poz_broj << " vidi kralja\n";
            pinnuj_ako_treba();
        }
            
    }

        void Kraljica :: obrisi_polja_koja_napada(){

            if(pinning!=NULL){
                pinning->unpinn();
                pinning=NULL;
            }

            for(std::pair< int, int> p : polja_koja_napada){
                std::cout<<tabla->figure_koje_napadaju_polje[p.first][p.second].erase(std::make_pair(poz_slovo,poz_broj));
                std::cout<<" Obrisano za Kraljicu"<<poz_slovo<<poz_broj<<" \n";
            }

            polja_koja_napada.clear();
        }