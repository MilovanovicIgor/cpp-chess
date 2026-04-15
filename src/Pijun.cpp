#include<iostream>
#include<cmath>
#include "Pijun.h"
#include<vector>
#include "Tabla.h"
#include "Figura.h"

        Pijun:: Pijun(int boja,Tabla* tabla,int slovo,int broj) 
            :Figura(slovo,broj,'p',boja)
        {
            this->tabla=tabla;
            this->En_Passant=false;
        }

        

        bool Pijun:: pomeri(int slovo, int broj){

            int tmp_slovo=poz_slovo;
            int tmp_broj=poz_broj;

            bool proslo=false;

            if(!validna_pozicija(slovo,broj)){
                return false;
            }
            else if(poz_slovo==slovo && (!pinned || ide_prema_pineru(slovo,broj))){
                proslo = idi_napred(slovo,broj);
                
            }
            else if(polja_koja_napada.find(std::make_pair(slovo,broj))!=polja_koja_napada.end() && (!pinned || jede_pinera(slovo,broj) || ide_prema_pineru(slovo,broj))){
                proslo = jedi2(slovo,broj);
            }
            if(proslo){
                std::cout<<"proslo\n";

                std::set<std::pair<int,int> > skup_za_iteraciju=tabla->figure_koje_napadaju_polje[tmp_slovo][tmp_broj];
                //Za svaku figuru koja je napadala prethodnu poziciju pijuna, azuriramo polja koja napadaju
                for(std::pair<int,int> p : skup_za_iteraciju){
                    std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                    
                    tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                    tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                }

                skup_za_iteraciju=tabla->figure_koje_napadaju_polje[poz_slovo][poz_broj];
                //Za svaku figuru koja napada novu poziciju pijuna, azuriramo polja koja napadaju
                for(std::pair<int,int> p : skup_za_iteraciju){
                    std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                    
                    tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                    tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                }
                
                //Odredjujemo polja koja napada pijun
                odredi_polja_koja_napada();

            }
            return proslo;
        }
        
        bool Pijun:: idi_napred(int slovo, int broj){
            std::cout<<"ide napred\n";
            if(tabla->polja[slovo][broj]!=NULL){
                return false;
            }
            if(boja==1){
                std::cout<<"beli\n";
                if(broj-poz_broj==2 && !pomeren){
                    if(tabla->polja[slovo][broj-1]!=NULL){
                    
                        return false;
                    }
                    else{
                        obrisi_polja_koja_napada();
                        
                        tabla->polja[poz_slovo][poz_broj]=NULL;
                        poz_broj=poz_broj+2;
                        tabla->polja[slovo][broj]=this;
                        this->En_Passant=true;
                        this->pomeren=true;
                        
                        return true;
                    }

                }
                else if(broj-poz_broj==1){
                    pomeren=true;
                    if(broj==7){
                        promocija(boja,slovo);

                    }
                    else{
                        obrisi_polja_koja_napada();

                        tabla->polja[poz_slovo][poz_broj]=NULL;
                        poz_broj=broj;
                        tabla->polja[poz_slovo][poz_broj]=this;
                        return true;
                    }
                }
                else {
                    std::cout<<"else\n";
                    return false;
                }
            }

            else if(boja==2){
                std::cout<<"crni\n";
                
                if(poz_broj-broj==2 && !pomeren){
                    if(tabla->polja[slovo][broj+1]!=NULL){
                        return false;
                    }
                    else{
                        obrisi_polja_koja_napada();

                        tabla->polja[poz_slovo][poz_broj]=NULL;
                        poz_broj=poz_broj-2;
                        tabla->polja[slovo][broj]=this;
                        this->En_Passant=true;
                        this->pomeren=true;
                        return true;

                    }

                }
                else if(poz_broj-broj==1){
                    pomeren=true;
                    if(broj==0){
                        promocija(boja,slovo);

                    }
                    else{
                        obrisi_polja_koja_napada();

                        tabla->polja[poz_slovo][poz_broj]=NULL;
                        poz_broj=broj;
                        tabla->polja[poz_slovo][poz_broj]=this;
                        return true;
                    }
                }
            }
        }

        bool Pijun:: jedi(int slovo,int broj){
            if(boja==1){
                if(broj-poz_broj!=1){
                    return false;
                }
                if(tabla->polja[slovo][broj]!=NULL && tabla->polja[slovo][broj]->vrati_boju()==2){
                    
                    if(!pinned || jede_pinera(slovo,broj)){
                        pomeren=true;
                        std::cout<<"Jede dobro\n";
                        tabla->polja[slovo][broj]=NULL;
                        tabla->polja[poz_slovo][poz_broj]=NULL;
                        if(broj==7){
                            
                            promocija(boja,slovo);
                            return true;
                        }
                        else{
                            std::cout<<"dobro ulazi\n";
                            poz_slovo=slovo;
                            poz_broj=broj;
                            tabla->polja[slovo][broj]=this;
                            return true;
                        }
                    }
                }
                else if(tabla->polja[slovo][broj]==NULL){
                    return odigraj_En_Passant(slovo,broj);
                }
                else return false;
            }

            else if(boja==2){
                if(poz_broj-broj!=1){
                    return false;
                }
                if(tabla->polja[slovo][broj]!=NULL && tabla->polja[slovo][broj]->vrati_boju()==1){
                    
                    if(!pinned || jede_pinera(slovo,broj)){
                        pomeren=true;
                        tabla->polja[slovo][broj]=NULL;
                        tabla->polja[poz_slovo][poz_broj]=NULL;
                        if(broj==0){
                            
                            promocija(boja,slovo);
                            return true;
                        }
                        else{
                            poz_slovo=slovo;
                            poz_broj=broj;
                            tabla->polja[slovo][broj]=this;
                            return true;
                        }
                    }
                }
                else if(tabla->polja[slovo][broj]==NULL){
                    return odigraj_En_Passant(slovo,broj);
                }
                else return false;
            }

        }

        bool Pijun:: jedi2(int slovo,int broj){

            if(tabla->polja[slovo][broj]==NULL && !pinned){
                return odigraj_En_Passant(slovo,broj);
            }
            else if(tabla->polja[slovo][broj]->vrati_boju()==this->vrati_boju()){
                return false;
            }

            else if(!pinned || jede_pinera(slovo,broj)) {
                //ovde treba da ispitam dolazi li do promocije
                obrisi_polja_koja_napada();
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

                tabla->polja[poz_slovo][poz_broj]=NULL;
                poz_slovo=slovo;
                poz_broj=broj;
                tabla->polja[poz_slovo][poz_broj]=this;
                return true;
            }
            else {
                return false;
            }

        }
        bool Pijun:: promocija(int boja, int slovo){
           /* std::cout<<"Unesite figuru [q,r,b,k]";
                        char c;
                        std::cin>>c;
                        
                        if(c=='q'){
                            Kraljica nova=Kraljica('q',boja);
                        }
                        else if(c=='r'){
                            Top nova=Top();
                        }
                        else if(c=='b'){
                            Lovac nova=Lovac();
                        }
                        else if(c=='k'){
                            Konj nova=Konj();
                        }
                        else return false;

                        tabla.polja[poz_slovo][poz_broj]=NULL; */
                        return true;
        }

        bool Pijun ::jede_pinera(int slovo,int broj){
            if(pinned_by==NULL){
                return false;
            }
            int slovo_pinnera=pinned_by->poz_slovo;
            int broj_pinnera=pinned_by->poz_broj;

            return (slovo==slovo_pinnera && broj==broj_pinnera);
        }

        bool Pijun :: ide_prema_pineru(int slovo,int broj){
            if(pinned_by==NULL){
                return false;
            }
            char oznaka_pinnera=pinned_by->vrati_oznaku();
            int poz_slovo_pinnera=pinned_by->poz_slovo;
            int poz_broj_pinnera=pinned_by->poz_broj;

            if(oznaka_pinnera=='b' || (oznaka_pinnera=='Q' && (poz_slovo!=poz_slovo_pinnera && poz_broj!=poz_broj_pinnera))){
                if(slovo==poz_slovo){
                    return false;
                }
                else if(poz_slovo_pinnera>poz_slovo && slovo>poz_slovo){
                    if(poz_broj_pinnera>poz_broj && boja==1){
                        return true;
                    }
                    else if(poz_broj_pinnera<poz_broj && boja==2){
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else if(poz_slovo_pinnera<poz_slovo && slovo<poz_slovo){
                    if(poz_broj_pinnera>poz_broj && boja==1){
                        return true;
                    }
                    else if(poz_broj_pinnera<poz_broj && boja==2){
                        return true;
                    }
                    else{
                        return false;
                    }
                }
            }
            else if(poz_slovo_pinnera==poz_slovo){
                if(slovo==poz_slovo){
                    return true;
                }
                else{
                    return false;
                }
            }
            else {
                return false;
            }
        }

        bool Pijun :: odigraj_En_Passant(int slovo,int broj){
            std::cout<<"Igra anpasant\n";
            if(boja==1 && tabla->polja[slovo][broj-1]!=NULL){
                std::cout<<"dodeljuje figuru\n";
                Figura *f=tabla->polja[slovo][broj-1];
                
                if(f->vrati_boju()==2 && f->vrati_oznaku()=='p' && f->dozvoljen_En_Passant() ){

                    obrisi_polja_koja_napada();
                    
                    Figura* f=tabla->polja[slovo][broj-1];
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
                    

                    tabla->polja[poz_slovo][poz_broj]=NULL;
                    tabla->polja[slovo][broj-1]=NULL;
                    poz_broj=broj;
                    poz_slovo=slovo;
                    tabla->polja[poz_slovo][poz_broj]=this;
                    return true;
                }
                return false;
            }
            else if(boja==2 && tabla->polja[slovo][broj+1]!=NULL) {
                Figura* f=(tabla->polja[slovo][broj+1]);
                if(f->vrati_boju()==1 && f->vrati_oznaku()=='p' && f->dozvoljen_En_Passant()){

                    obrisi_polja_koja_napada();
                    
                    Figura* f=tabla->polja[slovo][broj+1];
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
                    
                    tabla->polja[poz_slovo][poz_broj]=NULL;
                    tabla->polja[slovo][broj+1]=NULL;
                    poz_broj=broj;
                    poz_slovo=slovo;
                    tabla->polja[poz_slovo][poz_broj]=this;
                    return true;
                }
                return false;
            }
            else return false;
        }

        bool Pijun:: dozvoljen_En_Passant(){
            return  this->En_Passant;
        }
        void Pijun::onemoguci_En_Passant(){
            En_Passant=false;
        }

        bool Pijun:: vidi_kralja(){
           
        }

        void Pijun:: odredi_polja_koja_napada(){

            polja_koja_napada.clear();
            
            if(boja==1){

                int kandidat1_slovo=poz_slovo-1;
                int kandidat1_broj=poz_broj+1;

                int kandidat2_slovo=poz_slovo+1;
                int kandidat2_broj=poz_broj+1;

                if(validna_pozicija(kandidat1_slovo,kandidat1_broj)){
                    
                    this->polja_koja_napada.insert(std::make_pair(kandidat1_slovo,kandidat1_broj));
                    tabla->figure_koje_napadaju_polje[kandidat1_slovo][kandidat1_broj].insert(std::make_pair(poz_slovo,poz_broj));
                    
                }

                if(validna_pozicija(kandidat2_slovo,kandidat2_broj)){
                    
                    this->polja_koja_napada.insert(std::make_pair(kandidat2_slovo,kandidat2_broj));
                    tabla->figure_koje_napadaju_polje[kandidat2_slovo][kandidat2_broj].insert(std::make_pair(poz_slovo,poz_broj));

                }

            }
            if(boja==2){

                int kandidat1_slovo=poz_slovo-1;
                int kandidat1_broj=poz_broj-1;

                int kandidat2_slovo=poz_slovo+1;
                int kandidat2_broj=poz_broj-1;

                if(validna_pozicija(kandidat1_slovo,kandidat1_broj)){
                    
                    this->polja_koja_napada.insert(std::make_pair(kandidat1_slovo,kandidat1_broj));
                    
                }

                if(validna_pozicija(kandidat2_slovo,kandidat2_broj)){
                   
                    this->polja_koja_napada.insert(std::make_pair(kandidat2_slovo,kandidat2_broj));
                }

            }

            std::cout<<"Polja koja napada pijun"<<poz_slovo<<poz_broj<<"\n";
            for(std::pair< int, int> p : polja_koja_napada){
                std::cout<<p.first<<"  "<<p.second<<"\n";
            }
        }

        void Pijun :: obrisi_polja_koja_napada(){
            for(std::pair<int,int> p : polja_koja_napada){
                tabla->figure_koje_napadaju_polje[p.first][p.second].erase(std::make_pair(poz_slovo,poz_broj));
            }

            
            polja_koja_napada.clear();
        }

        

        
   