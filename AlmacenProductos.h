#pragma once
#include <string>
#include <vector>
#include "Producto.h"

class AlmacenProductos {
public:
	bool guardar(const std::string& ruta, const std::vector<Producto>& productos) const;
	bool cargar(const std::string& ruta, std::vector<Producto>& productos) const;
};
