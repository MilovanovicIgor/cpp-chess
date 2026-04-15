#ifndef KRALJ_H
#define KRALJ_H

#include "Figura.h"
#include "Tabla.h"

class Kralj :public Figura{
    public:

        Kralj(int boja,Tabla* tabla,int slovo,int broj);

        bool pomeri(int slovo,int broj);
        bool dozvoljen_En_Passant();
        void onemoguci_En_Passant();
        bool vidi_kralja();
        int vrati_poziciju_slovo();
        int vrati_poziciju_broj();
        void odredi_polja_koja_napada();
        void obrisi_polja_koja_napada();
        bool moze_se_pomeriti();

    private:
        Tabla* tabla;
        bool pomeren;
        
};




#endif