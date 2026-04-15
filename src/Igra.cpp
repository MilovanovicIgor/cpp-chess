#include<iostream>
#include<vector>
#include "Figura.h"
#include "Tabla.h"
#include "Pijun.h"
#include "Lovac.h"
#include "Top.h"
#include "Kraljica.h"
#include "Igra.h"


    
        Igra ::Igra(Tabla* t){
            this->tabla=t;
            this->i=1;
            this->beli_en_passant=NULL;
            this->crni_en_passant=NULL;
        }

        void Igra:: pocni_igru(){
            tabla->ispisi_tablu();
            
            while(true){

                if(tabla->sah_na_belom_kralju){
                    std::cout<<"Sah na belom kralju\n";
                }
                else if(tabla->sah_na_crnom_kralju){
                    std::cout<<"Sah na crnom kralju\n";
                }

              

                /*for(int i=0;i<8;i++){
                    for(int j=0;j<8;j++){
                        std::cout<<"Polje "<< i<< " "<< j<<" napadaju:\n";
                        for(std::pair<int,int> p: tabla->figure_koje_napadaju_polje[i][j]){
                            std::cout<< p.first<<" "<< p.second<< "\n";
                        }
                    }
                }*/
                    if(i==1){
                        std::cout<<"Beli";
                        if(beli_en_passant!=NULL){
                            beli_en_passant->onemoguci_En_Passant();
                            beli_en_passant=NULL;
                        }
                    }
                    else if(i==2){
                        std::cout<<"Crni";
                        if(crni_en_passant!=NULL){
                            crni_en_passant->onemoguci_En_Passant();
                            crni_en_passant=NULL;
                        }
                    }
                    
                    std::cout<<" igrac na potezu\n";
                    std::string s1,s2;
                    std::cin>>s1>>s2;
                    char oznaka=s1[0];
                    int poz_slovo=s1[1]-'a';
                    int poz_broj=s1[2]-'1';

                    int slovo=s2[0]-'a';
                    int broj=s2[1]-'1';

                    

                    if(tabla->polja[poz_slovo][poz_broj]!=NULL){
                        Figura *f=tabla->polja[poz_slovo][poz_broj];
                        

                        if(f->vrati_boju()!=i){
                            std::cout<<"Ne mozete pomeriti figuru protivnika\n";
                            tabla->ispisi_tablu();
                            continue;
                        }

                        if(i==1 && tabla->sah_na_belom_kralju && f->vrati_oznaku()!='K'){
                            if(!tabla->brani_sah(slovo,broj,i)){
                                std::cout<<"Morate odbraniti sah\n";

                                tabla->ispisi_tablu();
                                continue;
                            }
                        }
                        
                        else if(i==2 && tabla->sah_na_crnom_kralju && f->vrati_oznaku()!='K'){
                            if(!tabla->brani_sah(slovo,broj,i)){
                                std::cout<<"Morate odbraniti sah\n";

                                tabla->ispisi_tablu();
                                continue;
                            }
                        }

                        bool proslo=f->pomeri(slovo,broj);
                        if(!proslo){
                            std::cout<<"Nevalidan potez\n";
                            tabla->ispisi_tablu();
                        }
                        else{
                            //Potez je uspesno izvrsen

                            std::unordered_set<Figura *> moguci_pinneri;
                            //Ukoliko smo uspesno pomerili kralja, moramo da proverimo koje figure pinnuju koje figure sada
                            if(oznaka=='K'){
                                if(i==1){
                                    moguci_pinneri=tabla->moguci_crni_pinneri;
                                }
                                else{
                                    moguci_pinneri=tabla->moguci_beli_pinneri;
                                }

                                for(Figura * f: moguci_pinneri){

                                    if(f->vrati_oznaku()=='b'){

                                        Lovac* l=dynamic_cast<Lovac*> (f);

                                        if(l->pinning!=NULL){
                                            l->pinning->unpinn();
                                            l->pinning=NULL;
                                        }
                                        if(l->vidi_kralja()){
                                            l->pinnuj_ako_treba();
                                        }
                                    }
                                    else if(f->vrati_oznaku()=='r'){

                                        Top* t=dynamic_cast<Top*> (f);

                                        if(t->pinning!=NULL){
                                            t->pinning->unpinn();
                                            t->pinning=NULL;
                                        }
                                        if(t->vidi_kralja()){
                                            t->pinnuj_ako_treba();
                                        }
                                    }
                                    else if(f->vrati_oznaku()=='Q'){

                                        Kraljica* q=dynamic_cast<Kraljica*> (f);

                                        if(q->pinning!=NULL){
                                            q->pinning->unpinn();
                                            q->pinning=NULL;
                                        }
                                        if(q->vidi_kralja()){
                                            q->pinnuj_ako_treba();
                                        }
                                    }
                                }
                            }

                            
                            if(tabla->napadnuto_polje(tabla->Kralj_crni.first,tabla->Kralj_crni.second,2)){
                                tabla->sah_na_crnom_kralju=true;
                                if(tabla->sah_mat(2)){
                                    std::cout<<"SAH MAT!\n"<<"Beli je pobedio!\n";
                                    break;
                                }
                            }
                            else{
                                tabla->sah_na_crnom_kralju=false;
                                
                            }
                            

                            
                            if(tabla->napadnuto_polje(tabla->Kralj_beli.first,tabla->Kralj_beli.second,1)){
                                tabla->sah_na_belom_kralju=true;
                                if(tabla->sah_mat(1)){
                                    std::cout<<"SAH MAT!\n"<<"Crni je pobedio!\n";
                                    break;
                                }
                            }
                            else{
                                tabla->sah_na_belom_kralju=false;
                            }

                            
                            
                            //Ukoliko je pomerena figura bila pijun, proveravamo da li je moguc En_Passant i azuriramo ako jeste
                            if(s1[0]=='p'){
                                if(f->dozvoljen_En_Passant() && i==1){
                                    beli_en_passant=f;
                                }
                                else if(f->dozvoljen_En_Passant() && i==2){
                                    crni_en_passant=f;
                                }
                            }
                            //Menjamo igraca na potezu posto potez bio validan
                            i=3-i;
                            tabla->ispisi_tablu();
                        }
                    }
                    else{
                        std::cout<<"Prazno polje\n";
                    }

                
            }
        }
    
    
