#pragma once
#include <vector>
#include <string>
#include "Producto.h"

class GestorProductos {
private:
	std::vector<Producto> productos;

public:
	bool agregar(const Producto& nuevo);
	Producto* buscarPorCod(const std::string& cod);
	const std::vector<Producto>& getProductos() const;

	bool modificarPrecio(const std::string& cod, double nuevoPrecio);
	bool modificarNombre(const std::string& cod, const std::string& nuevoNombre);
	bool ingresaMerca(const std::string& cod, int cantidad);
	bool desactivar(const std::string& cod);
	void setProductos(const std::vector<Producto>& lista);
};
