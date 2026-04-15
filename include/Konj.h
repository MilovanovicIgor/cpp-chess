#ifndef KONJ_H
#define KONJ_H

#include "Figura.h"
#include "Tabla.h"

class Konj :public Figura{
    public:

        Konj(int boja,Tabla* tabla,int slovo,int broj);

        bool pomeri(int slovo,int broj);
        bool dozvoljen_En_Passant();
        void onemoguci_En_Passant();
        bool vidi_kralja();
        void odredi_polja_koja_napada();
        void obrisi_polja_koja_napada();
        

    private:
        Tabla* tabla;
       
};




#endif