#ifndef TABLA_H
#define TABLA_H

#include<vector>
#include<set>
#include<unordered_set>
#include "Figura.h"

bool validna_pozicija(int slovo, int broj);

class Tabla{
    public:
        
        std::vector<std::vector<Figura*> > polja;
        std::vector<std::vector<std::set<std::pair<int,int> > > > figure_koje_napadaju_polje;
        std::pair<int,int> Kralj_beli;
        bool sah_na_belom_kralju;
        std::pair<int,int> Kralj_crni;
        bool sah_na_crnom_kralju;
        std::pair<int,int> pozicija_en_passant;

        std::unordered_set<Figura *> moguci_beli_pinneri;
        std::unordered_set<Figura *> moguci_crni_pinneri;

        bool napadnuto_polje(int slovo,int broj,int boja);
        bool brani_sah(int slovo,int broj,int boja);
        bool sah_mat(int boja);
        std::set<std::pair<int, int>> polja_dijagonale(int slovo_poc,int broj_poc, int slovo_kraj,int broj_kraj);

    
        void postavi_pocetnu_poziciju();

        void ispisi_tablu();



        Figura* Figura_na_polju(char slovo, int broj);


};

#endif