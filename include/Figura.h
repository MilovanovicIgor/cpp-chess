#ifndef FIGURA_H
#define FIGURA_H

#include<set>

class Figura{
    public:
        int boja;
        int poz_slovo;
        int poz_broj;

        Figura(int slovo,int broj,char c, int boja);

        char vrati_oznaku();
        int vrati_boju();
        virtual bool dozvoljen_En_Passant();
        virtual void onemoguci_En_Passant();
        virtual bool pomeri(int slovo,int broj);
        virtual void odredi_polja_koja_napada();
        virtual void obrisi_polja_koja_napada();
        void pinn( Figura* f);
        void unpinn();
        bool pinned;
        Figura* pinned_by;
        std::set<std::pair<int,int> > polja_koja_napada;



    private:
        char oznaka;

        
       
};


#endif