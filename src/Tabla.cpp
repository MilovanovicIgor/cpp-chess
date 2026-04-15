#include<iostream>
#include<stdlib.h>
#include "Tabla.h"
#include "Pijun.h"
#include "Figura.h"
#include "Top.h"
#include "Lovac.h"
#include "Konj.h"
#include "Kraljica.h"
#include "Kralj.h"
#include<vector>
#include<set>
#include<cmath>


bool validna_pozicija(int slovo, int broj){
    return (slovo>=0 && slovo<=7 && broj>=0 && broj<=7);
}

        //Alocira matricu za tablu, i kreira objekte figura i postavlja ih na tacne pozicije za pocetak partije
        void Tabla ::postavi_pocetnu_poziciju(){

            sah_na_belom_kralju=false;
            sah_na_crnom_kralju=false;

            polja.resize(8);
            figure_koje_napadaju_polje.resize(8);

            for(int i = 0; i < 8; i++){
                
                polja[i].resize(8);
                figure_koje_napadaju_polje[i].resize(8);

                for(int j=0 ; j<8 ;j++){
                    if(j==1){
                        Pijun *p = new Pijun(1,this,i,j);
                        polja[i][j]=p;

                    }
                    else if(j==6){
                        Pijun *p=new Pijun(2,this,i,j);
                        polja[i][j]=p;
                    }
                    else if(j==0 && (i==0 || i==7)){
                        Top *t=new Top(1,this,i,j);
                        polja[i][j]=t;
                        moguci_beli_pinneri.insert(t);
                    }
                    else if(j==7 && (i==0 || i==7)){
                        Top *t=new Top(2,this,i,j);
                        polja[i][j]=t;
                        moguci_crni_pinneri.insert(t);
                    }
                    else if(j==0 && (i==2 || i==5)){
                        Lovac *l=new Lovac(1,this,i,j);
                        polja[i][j]=l;
                        moguci_beli_pinneri.insert(l);
                    }
                    else if(j==7 && (i==2 || i==5)){
                        Lovac *l=new Lovac(2,this,i,j);
                        polja[i][j]=l;
                        moguci_crni_pinneri.insert(l);
                    }
                    else if(j==0 && (i==1 || i==6)){
                        Konj *k=new Konj(1,this,i,j);
                        polja[i][j]=k;
                    }
                    else if(j==7 && (i==1 || i==6)){
                        Konj *k=new Konj(2,this,i,j);
                        polja[i][j]=k;
                    }
                    else if(j==0 && i==3){
                        Kraljica *Q=new Kraljica(1,this,i,j);
                        polja[i][j]=Q;
                        moguci_beli_pinneri.insert(Q);
                    }
                    else if(j==7 && i==3){
                        Kraljica *Q=new Kraljica(2,this,i,j);
                        polja[i][j]=Q;
                        moguci_crni_pinneri.insert(Q);
                    }
                    else if(j==0 && i==4){
                        Kralj *K=new Kralj(1,this,i,j);
                        polja[i][j]=K;
                        Kralj_beli=std::make_pair(i,j);
                    }
                    else if(j==7 && i==4){
                        Kralj *K=new Kralj(2,this,i,j);
                        polja[i][j]=K;
                        Kralj_crni=std::make_pair(i,j);
                    }
                    else polja[i][j]=NULL;
                 
                }
            }
            //Odredjuje za sve figure koje polja napadaju u pocetnoj poziciji
            for(int i=0 ; i<8 ; i++){
                for(int j=0 ; j<8 ;j++){
                    if(j==1 || j==6 || j==0 || j==7 ){
                        polja[i][j]->odredi_polja_koja_napada();
                    }
                }
            }
        }


        void Tabla::ispisi_tablu(){
                    std::cout<<"\t ";
                    for(int i=0;i<40;i++){
                        std::cout<<"-";
                    }
                    

                    for(int i=7;i>=0;i--){
                                                                             
                            std::cout<<"\n\t"<<i+1;
                            for(int k=0;k<40;k++){
                                if(k%5==0){
                                    std::cout<<"|";
                                }
                                else if(k%5==2){
                                    if(polja[k/5][i]==NULL){
                                        std::cout<<" ";
                                    }
                                    else {
                                        std::cout<<polja[k/5][i]->vrati_boju()<<polja[k/5][i]->vrati_oznaku();
                                        k++;
                                    }
                                }
                                else 
                                    std::cout<<" ";
                                
                            }
                            
                            
                            
                            std::cout<<"|\n\t ";
                            for(int k=0;k<40;k++){
                                std::cout<<"-";
                            }
                            
                        
                    }
                    std::cout<<"\n\t ";
                            char slovo='a';
                        for(int k=0;k<40;k++){
                                if(k%5==2){
                                    std::cout<<slovo;
                                    slovo++;
                                }
                                else{
                                    std::cout<<" ";
                                }
                        }
                    std::cout<<"\n";

                }



        Figura* Tabla:: Figura_na_polju(char slovo, int broj){
            if(broj>8 || broj <1) {
                std::cout<<"Los broj";
                exit(0);
            }

            else if(slovo<'a' || slovo >'h') {
                 std::cout<<"Lose slovo";
                 exit(0);
            }
            
            return polja[slovo-'a'][broj-1];
        }

//Vraca boolean vrednost koja oznacava da li je polje [slovo,broj] napadnuto od strane protivnicke figure (razlicite boje od one prosledjene u argumentu)
bool Tabla :: napadnuto_polje(int slovo,int broj,int boja){
    for(std::pair<int,int> p: figure_koje_napadaju_polje[slovo][broj]){
        if(polja[p.first][p.second]->vrati_boju()!=boja){
            return true;
        }
    }
      
}
//Ova f-ja prima tri argumenta: slovo i broj na koji se figura zeli pomeriti i njenu boju
//Ova f-ja se poziva ukoliko je sah trenutno, i treba je dozvoliti samo ukoliko posle ovog poteza vise nece biti sah
//Vraca true ili false u zavisnosti od toga da li bi taj potez odbranio sah na kralju
bool Tabla :: brani_sah(int slovo,int broj, int boja){
    int poz_kralja_broj;
    int poz_kralja_slovo;
    if(sah_na_belom_kralju){
        poz_kralja_slovo=Kralj_beli.first;
        poz_kralja_broj=Kralj_beli.second;
    }
    else if(sah_na_crnom_kralju){
        poz_kralja_slovo=Kralj_crni.first;
        poz_kralja_broj=Kralj_crni.second;
    }
        //Odredjujemo skup svih protivnickih figura koje napadaju kralja
        std::set<std::pair<int,int> > skup_koji_napada;
        for(std::pair<int,int> p : figure_koje_napadaju_polje[poz_kralja_slovo][poz_kralja_broj]){
            if(polja[p.first][p.second]->vrati_boju()!=boja){
                skup_koji_napada.insert(p);
            }
        }
        //Ukoliko vise od jedne figure napadaju kralja, sah je neodbranjiv pomeranjem druge figure
        if(skup_koji_napada.size()>1){
            return false;
        }
        //Ako kralja napada samo jedna figura
        else if(skup_koji_napada.size()==1){
            for(std:: pair<int , int> p :skup_koji_napada){

                int poz_napadaca_slovo=p.first;
                int poz_napadaca_broj=p.second;
                //izmeniti na oznaka_napadaca
                char oznaka=polja[poz_napadaca_slovo][poz_napadaca_broj]->vrati_oznaku();
                //Ukoliko je potez jedenje te figure, taj potez brani sah (proveriti da li se mora pinnovanje ispitati)
                if(poz_napadaca_slovo==slovo && poz_napadaca_broj==broj){
                    return true;
                }
                //Ukoliko ne jedemo figuru, sah je jedino odbranjiv ukoliko se figura pomera izmedju kralja i njenog napadaca, na liniji napada
                //Sah konjem ili pijunom je neodbranjiv drugom figurom
                else if(oznaka=='k' || oznaka=='p'){
                    return false;
                }
                //Ukoliko je napad horizontalan ili vertikalan topom ili kraljicom
                else if(oznaka=='r' || (oznaka=='Q' && (poz_napadaca_slovo==poz_kralja_slovo || poz_kralja_broj==poz_napadaca_broj))){
                    //Vertikalna linija napada
                    if(poz_napadaca_slovo==poz_kralja_slovo){
                        if(slovo!=poz_napadaca_slovo){
                            return false;
                        }
                        else if(broj>poz_kralja_broj && broj<poz_napadaca_broj){
                            return true;
                        }
                        else if(broj<poz_kralja_broj && broj>poz_napadaca_broj){
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                    //Horizontalna linija napada
                    else{
                        if(broj!=poz_kralja_broj){
                            return false;
                        }
                        else if(slovo>poz_kralja_slovo && slovo<poz_napadaca_slovo){
                            return true;
                        }
                        else if(slovo<poz_kralja_slovo && slovo>poz_napadaca_slovo){
                            return true;
                        }
                        else{
                            return false;
                        }
                    }
                }
                //Linija napada je dijagonalna, Lovcem ili Kraljicom
                else if(oznaka=='b' || oznaka=='Q'){
                    int razlika_slovo=slovo-poz_kralja_slovo;
                    int razlika_broj=broj-poz_kralja_broj;
                    if(std::abs(razlika_slovo)!=std::abs(razlika_broj)){
                        return false;
                    }
                    if(poz_napadaca_slovo>poz_kralja_slovo){
                        if(poz_napadaca_broj>poz_kralja_broj){
                            if(razlika_slovo>0 && razlika_broj>0 && razlika_slovo<poz_napadaca_slovo-poz_kralja_slovo){
                                return true;
                            }
                            
                        }
                        else if(poz_napadaca_broj<poz_kralja_broj){
                            if(razlika_slovo>0 && razlika_broj<0 && razlika_slovo<poz_napadaca_slovo-poz_kralja_slovo){
                                return true;
                            }
                        
                        }
                        else {
                            return false;
                        }

                    }
                    else{
                        if(poz_napadaca_broj>poz_kralja_broj){
                            if(razlika_slovo<0 && razlika_broj>0 && razlika_broj<poz_napadaca_broj-poz_kralja_broj){
                                return true;
                            }
                            else {
                                return false;
                            }
                            
                        }
                        else if(poz_napadaca_broj<poz_kralja_broj){
                            if(razlika_slovo<0 && razlika_broj<0 && razlika_slovo>poz_napadaca_slovo-poz_kralja_slovo){
                                return true;
                            }
                        
                        }
                        else {
                            return false;
                        }
                    } 
                    
                }

            }
        }
        else{
            std::cout<<"\n\nNesto nije u redu\n\n";
            return false;
        }
}
        //Vraca skup parova polja odredjenih sa [slovo,broj] koji se nalaze na dijagonalnoj liniji izmedju dve pozicije
        //Kao argumente dobija pocetnu i krajnju poziciju, tj  [slovo_poc,broj_poc] i [slovo_kraj,broj_kraj]
        std::set<std::pair<int, int>> Tabla :: polja_dijagonale(int slovo_poc,int broj_poc, int slovo_kraj,int broj_kraj){
            int pom_slovo,pom_broj;
            if(slovo_poc<slovo_kraj){
                pom_slovo=1;
            }
            else{
                pom_slovo=-1;
            }
            if(broj_poc<broj_kraj){
                pom_broj=1;
            }
            else{
                pom_broj=-1;
            }
            std::set<std::pair<int, int > > skup_polja_izmedju;
            for(int i=slovo_poc+pom_slovo,j=broj_poc+pom_broj;i!=slovo_kraj && j!=broj_kraj;i+=pom_slovo,j+=pom_broj){
                skup_polja_izmedju.insert(std::make_pair(i,j));
            }
            return skup_polja_izmedju;
        }


//Ova f-ja proverava da li je doslo do sah-mata. Kao argument prima boju kralja koji se ispituje.
bool Tabla :: sah_mat(int boja){
    std::cout<<"Uslo u proveru sah-mata\n";
    Kralj* k;
    if(boja==1){
        if(!sah_na_belom_kralju){
           return false; 
        }
        
        k=dynamic_cast<Kralj*>(polja[Kralj_beli.first][Kralj_beli.second]);
    }
    else{
        if(!sah_na_crnom_kralju){
            return false;
        }
        std::cout<<polja[Kralj_crni.first][Kralj_crni.second]->vrati_oznaku()<<" oznaka\n";
        k=dynamic_cast<Kralj*>(polja[Kralj_crni.first][Kralj_crni.second]);
    }
    std::cout<<"Uslo u proveru sah-mata 1.5\n";
    std::cout<<k->vrati_oznaku()<<"\n";

    
    if(k->moze_se_pomeriti()){
        return false;
    }

    Figura* napadac;
    int brojac=0;
    //Odredjujemo figuru koja napada kralja. Ukoliko ih je vise, a kralj ne moze da se pomeri, mat je
    for(std::pair<int,int> p : figure_koje_napadaju_polje[k->poz_slovo][k->poz_broj]){
        if(polja[p.first][p.second]->vrati_boju()!=boja){
            brojac++;
            if(brojac>1){
                return true;
            }
            napadac=polja[p.first][p.second];
            
        }
    }
    std::cout<<"Uslo u proveru sah-mata 3\n";
    //Proveravamo da li napadaca na kralja, napada neka figura kraljeve boje.
    //Ukoliko postoji, i ta figura nije pinnova, moze je pojesti te ne dolazi do sah mata.
    for(std::pair<int,int> p : figure_koje_napadaju_polje[napadac->poz_slovo][napadac->poz_broj]){
        Figura* f=polja[p.first][p.second];
        if(f->vrati_boju()==boja && !f->pinned && f->vrati_oznaku()!='K'){
            return false;
        }
    }
    std::cout<<"Uslo u proveru sah-mata 4\n";
    //Zakljucujemo da kralj ne moze da se pomeri i ne postoji figura koje napada napadaca na kralja.
    //Jedina moguca opcija, a da nije sah mat jeste da postoji figura koja nije pinnovana, i moze se pomeriti izmedju napadaca
    //i kralja, tj odbraniti napad

    //Ukoliko je napadac konj, to nije moguce
    if(napadac->vrati_oznaku()=='k'){
        return true;
    }
    //Proveravamo da li je napad na kralja vertikalnom linijom
    else if(napadac->poz_slovo==k->poz_slovo){
        int min=std::min(napadac->poz_broj,k->poz_broj);
        int max=std::max(napadac->poz_broj,k->poz_broj);

        for(int i=min+1;i<max;i++){
            for(std::pair<int,int> p : figure_koje_napadaju_polje[napadac->poz_slovo][i]){
                Figura* f=polja[p.first][p.second];
                if(f->vrati_boju()==boja && !f->pinned && f->vrati_oznaku()!='K'){
                    return false;
                }
            } 
        }
    }
    //Proveravamo dali je napad na kralja horizontalnom linijom
    else if(napadac->poz_broj==k->poz_broj){
        std::cout<<"Uslo u proveru sah-mata 5\n";
        int min=std::min(napadac->poz_slovo,k->poz_slovo);
        int max=std::max(napadac->poz_slovo,k->poz_slovo);

        for(int i=min+1;i<max;i++){
            for(std::pair<int,int> p : figure_koje_napadaju_polje[i][napadac->poz_broj]){
                Figura* f=polja[p.first][p.second];
                if(f->vrati_boju()==boja && !f->pinned && f->vrati_oznaku()!='K'){
                    return false;
                }
            } 
        }
    }
    //Napad na kralja je preko dijagonale
    else{
        std::cout<<"Uslo u proveru sah-mata 6\n";
        std::set<std::pair<int, int > > skup_polja_izmedju=polja_dijagonale(napadac->poz_slovo,napadac->poz_broj,k->poz_slovo,k->poz_broj);
        for(std::pair<int,int> p1 : skup_polja_izmedju){
            for(std::pair<int, int> p2 : figure_koje_napadaju_polje[p1.first][p1.second] ){
                Figura* f=polja[p2.first][p2.second];
                if(f->vrati_boju()==boja && !f->pinned && f->vrati_oznaku()!='K'){
                    return false;
                }
            }
        }
    }
    return true;
    
    
}

