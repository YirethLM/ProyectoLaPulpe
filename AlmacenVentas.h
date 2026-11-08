
#pragma once
#include "Venta.h"
#include <vector>
#include <string>
using namespace std;

//Clase para guardar y cargar las ventas en JSON:joz1303

class AlmacenVentas {
public:

    /*Guarda las ventas en un archivo JSON : joz1303*/
    bool guardar(const string& ruta, const vector<Venta>& ventas) const;

    //Carga las ventas que estaban guardadas:joz1303
    bool cargar(const string& ruta, vector<Venta>& ventas) const;

};
