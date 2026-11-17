
#include "GestorHistorial.h"

//Constructor que guarda la ruta del archivo JSON:joz1303
GestorHistorial::GestorHistorial(const string& ruta)
    : ruta(ruta) {
}

//Carga las ventas que estaban guardadas en el JSON:joz1303
bool GestorHistorial::cargarVentas() {
    return almacen.cargar(ruta, ventas);
}

//Guarda las ventas actuales en el archivo JSON:joz1303
bool GestorHistorial::guardarVentas() {
    return almacen.guardar(ruta, ventas);
}

//Agrega una nueva venta al historial en memoria:joz1303
void GestorHistorial::agregarVenta(const Venta& venta) {
    ventas.push_back(venta);
    //push_back agrega la venta al vector de ventas:joz1303
}

//Devuelve todas las ventas para poder consultarlas:joz1303
const vector<Venta>& GestorHistorial::consultarVentas() const {
    return ventas;
}

//Busca una venta comparando su ID con el solicitado:joz1303
const Venta* GestorHistorial::buscarPorId(int id) const {

    for (const Venta& venta : ventas) {

        //Si encontramos el mismo ID, devolvemos esa venta:joz1303
        if (venta.getId() == id) {
            return &venta;
        }
    }

    //Si no encontramos la venta, devolvemos nullptr:joz1303
    return nullptr;
}
/*Gracias por usar nuestro sistema :) ¿Burpy o jules?*/
