#include "../LaPulpe_productos/GestorProductos.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

int main() {
    GestorProductos gestor;
    assert(gestor.agregar(Producto("P001", "Arroz", 1500, 20)));
    assert(gestor.getProductos().size() == 1);
    assert(!gestor.agregar(Producto("P001", "Duplicado", 100, 1)));
    assert(!gestor.agregar(Producto(" ", "Arroz", 100, 1)));
    assert(!gestor.agregar(Producto("P002", "   ", 100, 1)));
    assert(!gestor.agregar(Producto("P002", "Frijoles", 0, 1)));
    assert(!gestor.agregar(Producto("P002", "Frijoles", -2, 1)));
    assert(!gestor.agregar(Producto("P002", "Frijoles", 100, -1)));
    assert(!gestor.agregar(Producto("P002", "Frijoles", std::numeric_limits<double>::quiet_NaN(), 1)));
    assert(gestor.buscarPorCod("INEXISTENTE") == nullptr);
    assert(gestor.modificarNombre("P001", "Arroz integral"));
    assert(gestor.buscarPorCod("P001")->getNombre() == "Arroz integral");
    assert(!gestor.modificarNombre("P001", "  "));
    assert(gestor.modificarPrecio("P001", 1800));
    assert(!gestor.modificarPrecio("P001", std::numeric_limits<double>::infinity()));
    assert(!gestor.modificarPrecio("P001", -1));
    assert(gestor.buscarPorCod("P001")->getPrecio() == 1800);
    assert(gestor.ingresaMerca("P001", 5));
    assert(gestor.buscarPorCod("P001")->getExistencia() == 25);
    assert(!gestor.ingresaMerca("P001", 0));
    assert(!gestor.ingresaMerca("P001", std::numeric_limits<int>::max()));
    assert(gestor.buscarPorCod("P001")->getExistencia() == 25);
    assert(!gestor.buscarPorCod("P001")->rebajarExistencia(26));
    assert(gestor.buscarPorCod("P001")->rebajarExistencia(3));
    assert(gestor.buscarPorCod("P001")->getExistencia() == 22);
    assert(gestor.desactivar("P001"));
    assert(!gestor.desactivar("P001"));
    assert(!gestor.modificarPrecio("P001", 2000));
    assert(!gestor.modificarNombre("P001", "Otro"));
    assert(!gestor.ingresaMerca("P001", 2));
    assert(!gestor.buscarPorCod("P001")->rebajarExistencia(1));
    std::cout << "Pruebas de productos completadas correctamente\n";
}
