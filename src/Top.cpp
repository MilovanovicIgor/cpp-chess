
#include<iostream>
#include<vector>
#include<cmath>
#include "Figura.h"
#include "Tabla.h"
#include "Top.h"

        Top::Top(int boja,Tabla* tabla,int slovo,int broj):Figura(slovo,broj,'r',boja){
            this->tabla=tabla;
            this->pomeren=false;
            this->pinning=NULL;
        }

        bool Top:: pomeri(int slovo,int broj){

            int tmp_slovo=poz_slovo;
            int tmp_broj=poz_broj;
            //Ako se zeljena pozicija ne nalazi na istoj horizontali ili vertikali od trenutne, potez je nevalidan
            if(poz_slovo!=slovo && poz_broj!=broj){
                std::cout<<"Prvi if";
                return false;
            }
            //Ukoliko top napada zeljeno polje
            else if(polja_koja_napada.find(std::make_pair(slovo,broj))!=polja_koja_napada.end()){
                //Top se moze pomeriti na zeljenu poziciju ukoliko je polje prazno ili je na njemu protivnicka figura i ako nije pinovan ili mu taj potez pin dozvoljava
                if((!pinned || jede_pinera(slovo,broj) || ide_prema_pineru(slovo,broj)) && (tabla->polja[slovo][broj]==NULL || tabla->polja[slovo][broj]->vrati_boju()!=this->vrati_boju())){

                    obrisi_polja_koja_napada();
                    if(tabla->polja[slovo][broj]!=NULL){
                        //Top jede protivnicku figuru
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
                    //Azuriramo poziciju topa na novu i oslobadjamo polje na kojem je bio
                    tabla->polja[poz_slovo][poz_broj]=NULL;
                    poz_slovo=slovo;
                    poz_broj=broj;
                    tabla->polja[poz_slovo][poz_broj]=this;

                    pomeren=true;
                    
                    std::set<std::pair<int,int> > skup_za_iteraciju=tabla->figure_koje_napadaju_polje[tmp_slovo][tmp_broj];

                    //Za svaku figuru koja je napadala prehodnu poziciju topa, azuriramo polja koja napada
                    for(std::pair<int,int> p : skup_za_iteraciju){
                        std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                        
                        tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                        tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                    }
                    
                    skup_za_iteraciju=tabla->figure_koje_napadaju_polje[poz_slovo][poz_broj];
                    //Za svaku figuru koja je napada novu poziciju topa, azuriramo polja koja napada
                    for(std::pair<int,int> p : skup_za_iteraciju){
                        std::cout<<"Uslo\n"<<p.first<<"  "<<p.second;
                        
                        tabla->polja[p.first][p.second]->obrisi_polja_koja_napada();
                        tabla->polja[p.first][p.second]->odredi_polja_koja_napada();
                    }
                    
                    //Azuriramo polja koja top napada
                    odredi_polja_koja_napada();
                    return true;
                }
                else{
                    return false;
                }

            }
            else{
                for(std::pair<int,int> p:polja_koja_napada){
                    std::cout<<p.first<<p.second<<"\n";
                }
                return false;
            }
            
        }
        bool Top::dozvoljen_En_Passant(){return false;}

        void Top::onemoguci_En_Passant(){}
        
        bool Top::jede_pinera (int slovo,int broj){
            if(pinned_by==NULL){
                return false;
            }
            int slovo_pinnera=pinned_by->poz_slovo;
            int broj_pinnera=pinned_by->poz_broj;

            return (slovo==slovo_pinnera && broj==broj_pinnera);
            
        }
        //Ova metoda prima 2 argumenta, poziciju polja na koju zelimo da se top pomeri
        //Vraca true ukoliko nije pinovana ili se pomera na liniji na koje je pinovana, u suprotnom vraca false
        bool Top:: ide_prema_pineru(int slovo,int broj){
            if(pinned_by==NULL){
                return false;
            }
            char oznaka_pinnera=pinned_by->vrati_oznaku();
            int poz_slovo_pinnera=pinned_by->poz_slovo;
            int poz_broj_pinnera=pinned_by->poz_broj;
            //Ukoliko je top pinnovan po dijagonali, pomeranje nije moguce
            if(oznaka_pinnera=='b' || (oznaka_pinnera=='Q' && (poz_slovo_pinnera!=poz_slovo && poz_broj_pinnera!=poz_broj_pinnera))){
                return false;
            }
            //Ukoliko je top pinnovan po vertikali
            else if(poz_slovo==poz_slovo_pinnera){

                int poz_broj_kralja;
                if(boja==1){
                    poz_broj_kralja=tabla->Kralj_beli.second;
                }
                else{
                    poz_broj_kralja=tabla->Kralj_crni.second;
                }

                return (broj>std::min(poz_broj_kralja,poz_broj_pinnera) && broj<std::max(poz_broj_kralja,poz_broj_pinnera));
            }
            //Ukoliko je top pinnovan po horizontali
            else{
                int poz_slovo_kralja;
                if(boja==1){
                    poz_slovo_kralja=tabla->Kralj_beli.first;
                }
                else{
                    poz_slovo_kralja=tabla->Kralj_crni.first;
                }

                return (slovo>std::min(poz_slovo_kralja,poz_slovo_pinnera) && slovo<std::max(poz_slovo_kralja,poz_slovo_pinnera));
            }
            
        }
        
        bool Top::jedi(int slovo,int broj){
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
        //Vraca true ili false ukoliko je protivnicki kralj na liniji napada topa, nezavisno od drugih figura izmedju njih
        bool Top:: vidi_kralja(){
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
            return (poz_slovo==poz_kralja_slovo || poz_broj==poz_kralja_broj);
        }
        //Ova metoda odredjuje da li je neka figura pinnovana topom, i ukoliko jeste pinnuje je
        void Top :: pinnuj_ako_treba(){
            
            int poz_slovo_kralja,poz_broj_kralja;

            if(boja==1){
                poz_slovo_kralja=tabla->Kralj_crni.first;
                poz_broj_kralja=tabla->Kralj_crni.second;
            }
            else{
                poz_slovo_kralja=tabla->Kralj_beli.first;
                poz_broj_kralja=tabla->Kralj_beli.second;
            }
            //Kandidat je svaka protivnicka figura na liniji napada izmedju topa i kralja
            Figura* kandidat=NULL;
            //Ukoliko se top i protivnicki kralj nalaze na istoj vertikali, pinnuje se protivnicka figura ukoliko je jedina na liniji napada izmedju topa i kralja
            if(poz_slovo_kralja==poz_slovo){
                int bottom=std::min(poz_broj_kralja,poz_broj);
                int top=std::max(poz_broj_kralja,poz_broj);
                for(int i=bottom+1;i<top;i++){
                    Figura * f=tabla->polja[poz_slovo][i];
                    if(f!=NULL){
                        if(f->boja==boja){
                            //Na liniji napada se nalazi figura iste boje kao i top
                            return;
                        }
                        else if(kandidat!=NULL){
                            //Na liniji napada se nalazi vise od 1 protivnicke figure
                            return;
                        }
                        else{
                            kandidat=f;
                        }
                    }
                }
            }
            //Ukoliko se top i protivnicki kralj nalaze na istoj vertikali, pinnuje se protivnicka figura ukoliko je jedina na liniji napada izmedju topa i kralja
            else if(poz_broj_kralja==poz_broj){
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
            //Ukoliko smo pronasli kandidata, pinnujemo ga
            if(kandidat!=NULL){
                kandidat->pinn(this);
                this->pinning=kandidat;
            }
        }

        void Top :: odredi_polja_koja_napada(){

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

            std::cout<<"Polja koja napada top"<<poz_slovo<<poz_broj<<"\n";
            for(std::pair< int, int> p : polja_koja_napada){
                std::cout<<p.first<<"  "<<p.second<<"\n";
            }

            if(vidi_kralja()){
                std::cout<<this->vrati_oznaku()<<" "<< poz_slovo<< " "<<poz_broj << " vidi kralja\n";
                pinnuj_ako_treba();
            }

        }

        void Top :: obrisi_polja_koja_napada(){

            if(pinning!=NULL){
                pinning->unpinn();
                pinning=NULL;
            }

            for(std::pair< int, int> p : polja_koja_napada){
                std::cout<<tabla->figure_koje_napadaju_polje[p.first][p.second].erase(std::make_pair(poz_slovo,poz_broj));
                std::cout<<" Obrisano za top"<<poz_slovo<<poz_broj<<" \n";
            }

            polja_koja_napada.clear();
        }

bool Top :: getPomeren(){
    return pomeren;
}

void Top :: setPozSlovo(int slovo){
    poz_slovo=slovo;
}

void Top :: setPozBroj(int broj){
    poz_broj=broj;
}
