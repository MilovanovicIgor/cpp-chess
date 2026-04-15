#include<iostream>
#include<vector>
#include "Figura.h"
#include "Tabla.h"
#include "Lovac.h"
#include<cmath>

    Lovac::Lovac(int boja,Tabla* tabla,int slovo,int broj):Figura(slovo,broj,'b',boja){
        this->tabla=tabla;
        this->pinning=NULL;
    }

    bool Lovac:: pomeri(int slovo,int broj){
        int tmp_slovo=poz_slovo;
        int tmp_broj=poz_broj;

            
        if(polja_koja_napada.find(std::make_pair(slovo,broj))!=polja_koja_napada.end()){
            //Lovac napada zeljeno polje za pomeraj
            std::cout<<"Usao";
            
            if((!pinned || jede_pinera(slovo,broj) || ide_prema_pineru(slovo,broj)) && (tabla->polja[slovo][broj]==NULL || tabla->polja[slovo][broj]->vrati_boju()!=this->vrati_boju())){
                //Lovac se moze pomeriti na zeljenu poziciju ukoliko je polje prazno ili je na njemu protivnicka figura i ako nije pinovan ili mu taj potez pin dozvoljava.
                obrisi_polja_koja_napada();
                if(tabla->polja[slovo][broj]!=NULL){
                    //Lovac jede protivnicku figuru
                    Figura* f=tabla->polja[slovo][broj];
                    f->obrisi_polja_koja_napada();
                    char c=f->vrati_oznaku();

                    //Ukoliko je protivnicka figura Kraljica,Lovac ili Top, brisemo ih iz skupa mogucih pinnera
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
                //Azuriramo poziciju lovca na novu, i oslobadjamo polje na kojem je bio
                tabla->polja[poz_slovo][poz_broj]=NULL;
                poz_slovo=slovo;
                poz_broj=broj;
                tabla->polja[poz_slovo][poz_broj]=this;

                std::set<std::pair<int,int> > skup_za_iteraciju=tabla->figure_koje_napadaju_polje[tmp_slovo][tmp_broj];
                //Za svaku figuru koja je napadala prehodnu poziciju lovca, azuriramo polja koja napada
                for(std::pair<int,int> p : skup_za_iteraciju){
                    
                    tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                    tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                }

                skup_za_iteraciju=tabla->figure_koje_napadaju_polje[poz_slovo][poz_broj];
                //Za svaku figuru koja je napada novu poziciju lovca, azuriramo polja koja napada   
                for(std::pair<int,int> p : skup_za_iteraciju){
                    
                    tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                    tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                }
                    
                //Azuriramo polja koja lovac napada
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
    bool Lovac::dozvoljen_En_Passant(){return false;}
    void Lovac::onemoguci_En_Passant(){}
    bool Lovac::jedi(int slovo,int broj){
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
        else return false;
    }
    bool Lovac::jede_pinera(int slovo,int broj){
        if(pinned_by==NULL){
            return false;
        }
        int slovo_pinnera=pinned_by->poz_slovo;
        int broj_pinnera=pinned_by->poz_broj;

        return (slovo==slovo_pinnera && broj==broj_pinnera);
    }
    //Ova metoda prima 2 argumenta, poziciju polja na koju zelimo da se lovac pomeri
    //Vraca true ukoliko nije pinovana ili se pomera na liniji na koje je pinovana, u suprotnom vraca false
    bool Lovac :: ide_prema_pineru(int slovo,int broj){
        if(pinned_by==NULL){
            return false;
        }
        char oznaka_pinnera=pinned_by->vrati_oznaku();
        int poz_slovo_pinnera=pinned_by->poz_slovo;
        int poz_broj_pinnera=pinned_by->poz_broj;
        //Ukoliko je lovac pinnovan po vertikali ili horizontali, nije moguce da ide linijom napada
        if(oznaka_pinnera=='r' || (oznaka_pinnera=='Q' && (poz_slovo_pinnera==poz_slovo && poz_broj_pinnera==poz_broj_pinnera))){
            return false;
        }
        //Linija napada je dijagonala
        else {
            int poz_kralja_slovo,poz_kralja_broj;

            if(boja==1){
                poz_kralja_slovo=tabla->Kralj_beli.first;
                poz_kralja_broj=tabla->Kralj_beli.second;
            }
            else{
                poz_kralja_slovo=tabla->Kralj_crni.first;
                poz_kralja_broj=tabla->Kralj_crni.second;
            }
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
                    //valjda treba ovde else false
                            
                }
                else if(poz_broj_pinnera<poz_kralja_broj){
                    if(razlika_slovo>0 && razlika_broj<0 && razlika_slovo<poz_slovo_pinnera-poz_kralja_slovo){
                        return true;
                    }
                    //isto ovde
                        
                }
                else {
                    //ovo mislim ne treba
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


            
            
    }
        //Vraca true ili false u zavisnosti da li se protivnicki kralj nalazi u liniji napada ove figure, nezavisno od toga da li izmedju njih
        //postoje druge figure
    bool Lovac::vidi_kralja(){

        int poz_kralja_slovo;
        int poz_kralja_broj;
        //Ucitava kordinate protivnickog kralja    
        if(boja==1){
            poz_kralja_slovo=tabla->Kralj_crni.first;
            poz_kralja_broj=tabla->Kralj_crni.second;
            }
        else{
            poz_kralja_slovo=tabla->Kralj_beli.first;
            poz_kralja_broj=tabla->Kralj_beli.second;
        }

        int razlika_slovo=std::abs(poz_slovo-poz_kralja_slovo);
        int razlika_broj=std::abs(poz_broj-poz_kralja_broj);
        //vidi ga samo ako se nalaze na istoj dijagonali
        return razlika_slovo==razlika_broj;
    }

    void Lovac :: pinnuj_ako_treba(){

        int poz_slovo_kralja,poz_broj_kralja;
        //Ucitavamo koordinate protivnickog kralja
        if(boja==1){
            poz_slovo_kralja=tabla->Kralj_crni.first;
            poz_broj_kralja=tabla->Kralj_crni.second;
        }
        else{
            poz_slovo_kralja=tabla->Kralj_beli.first;
            poz_broj_kralja=tabla->Kralj_beli.second;
        }

        Figura* kandidat=NULL;
        //Kandidat za pinovanu figuru je samo prva protivnicka figura, racunajuci od kralja, koja se nalazi na liniji napada ove figure na protivnickog kralja
        //Medjutim, taj kandidat nece biti pinnovan, ukoliko na liniji napada postoji jos neka druga figura, bilo koje boje, te samo izlazimo iz f-je sa return
        if(poz_slovo>poz_slovo_kralja && poz_broj > poz_broj_kralja){
            
            for(int i=poz_slovo_kralja+1,j=poz_broj_kralja+1;i<poz_slovo && j< poz_broj; i++,j++){
                Figura* f=tabla->polja[i][j];
                if(f!=NULL){
                    if(f->boja==boja){
                        //Ako se nasla figura iste boje kao ova, pinna nema
                        return;
                    }
                    else if(kandidat!=NULL){
                        //Pored kandidata, nasla se jos jedna protivnicka figura, tako da nijedna od njih nije pinnovana
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

        if(kandidat!=NULL){
            //Posto nismo izasli iz f-je, znaci da je na liniji napada samo jedna protivnicka figura, te je pinnujemo
            kandidat->pinn(this);
            this->pinning=kandidat;
        }

    }

    void Lovac :: odredi_polja_koja_napada(){

        polja_koja_napada.clear();

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

    void Lovac :: obrisi_polja_koja_napada(){

        if(pinning!=NULL){
            pinning->unpinn();
            pinning=NULL;
        }

        for(std::pair< int, int> p : polja_koja_napada){
                std::cout<<tabla->figure_koje_napadaju_polje[p.first][p.second].erase(std::make_pair(poz_slovo,poz_broj));
                std::cout<<" Obrisano za lovca"<<poz_slovo<<poz_broj<<" \n";

            }

            polja_koja_napada.clear();
    }

