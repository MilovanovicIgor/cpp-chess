#ifndef LOVAC_H
#define LOVAC_H

#include "Figura.h"
#include "Tabla.h"

class Lovac :public Figura{
    public:

        Lovac(int boja,Tabla* tabla,int slovo,int broj);
        Figura* pinning;
        bool pomeri(int slovo,int broj);
        bool dozvoljen_En_Passant();
        void onemoguci_En_Passant();
        bool jedi(int slovo,int broj);
        bool jede_pinera(int slovo,int broj);
        bool ide_prema_pineru(int slovo,int broj);
        void pinnuj_ako_treba();
        bool vidi_kralja();
        void odredi_polja_koja_napada();
        void obrisi_polja_koja_napada();

    private:
        Tabla* tabla;
        
};




#endif