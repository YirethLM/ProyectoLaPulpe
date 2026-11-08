
#pragma once
#include "Venta.h"
#include "AlmacenVentas.h"
#include <vector>
#include <string>
using namespace std;

//Clase para administrar el historial de ventas:joz1303
class GestorHistorial {
private:
    vector<Venta> ventas; //Guarda las ventas en memoria:joz1303
    AlmacenVentas almacen; //Permite guardar y cargar el JSON:joz1303
    string ruta; //Ubicacion del archivo ventas.json:joz1303

public:
    //Constructor que recibe la ruta del archivo:joz1303
    GestorHistorial(const string& ruta);

    //Carga las ventas guardadas anteriormente:joz1303
    bool cargarVentas();

    //Guarda las ventas actuales en JSON:joz1303
    bool guardarVentas();

    //Agrega una venta nueva al historial:joz1303
    void agregarVenta(const Venta& venta);

    //Devuelve todas las ventas del historial:joz1303
    const vector<Venta>& consultarVentas() const;

    //Busca una venta usando su numero de identificacion:joz1303
    const Venta* buscarPorId(int id) const;
};
