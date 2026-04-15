#ifndef IGRA_H
#define IGRA_H

#include "Tabla.h"
#include "Figura.h"
class Igra{
    public:
    Igra(Tabla* t);

    void pocni_igru();

    private:
        Tabla *tabla;
        int i;
        Figura* crni_en_passant;
        Figura* beli_en_passant;

};


#endif