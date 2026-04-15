
#ifndef PIJUN_H
#define PIJUN_H

#include "Tabla.h"
#include "Figura.h"

class Pijun: public Figura{
    public:
        Pijun(int boja,Tabla* tabla,int slovo,int broj);

        bool pomeri(int slovo, int broj);

        bool idi_napred(int slovo, int broj);

        bool jedi(int slovo,int broj);

        bool jedi2(int slovo,int broj);
                    
        bool promocija(int boja, int slovo);

        bool jede_pinera(int slovo,int broj);

        bool ide_prema_pineru(int slovo,int broj);

        bool odigraj_En_Passant(int slovo,int broj);

        bool dozvoljen_En_Passant();

        void onemoguci_En_Passant();

        bool vidi_kralja();

        void odredi_polja_koja_napada();

        void obrisi_polja_koja_napada();

        
    private:
    Tabla* tabla;
    bool En_Passant;
    

    bool pomeren;
};

#endif