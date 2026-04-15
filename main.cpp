#include<iostream>
#include<vector>
#include<stdlib.h>
#include<cmath>
#include<string>

#include "Tabla.h"
#include "Igra.h"



int main(){
    std::cout<<"Pokrenut\n";

    Tabla tabla=Tabla();

    tabla.postavi_pocetnu_poziciju();
    
    Igra igra=Igra(&tabla);
    igra.pocni_igru();

    
    return 0;
}