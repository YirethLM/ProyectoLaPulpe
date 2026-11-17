#include "GestorProductos.h"
#include <cmath>
#include <algorithm>
#include <cctype>

namespace {
bool tieneTexto(const std::string& texto) {
	return std::any_of(texto.begin(), texto.end(), [](unsigned char c) {
		return !std::isspace(c);
	});
}
}

bool GestorProductos::agregar(const Producto& nuevo) {
	if (!tieneTexto(nuevo.getCodigo()) || !tieneTexto(nuevo.getNombre())) return false;
	if (!std::isfinite(nuevo.getPrecio()) || nuevo.getPrecio() <= 0 || nuevo.getExistencia() < 0 || !nuevo.getActivo()) return false;
	if (buscarPorCod(nuevo.getCodigo()) != nullptr) return false;
	productos.push_back(nuevo);
	return true;
}

Producto* GestorProductos::buscarPorCod(const std::string& cod) {
	for (Producto& p : productos) {
		if (p.getCodigo() == cod) return &p;
	}
	return nullptr;
}

const std::vector<Producto>& GestorProductos::getProductos() const {
	return productos;
}

bool GestorProductos::modificarPrecio(const std::string& cod, double nuevoPrecio) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr || !p->getActivo() || !std::isfinite(nuevoPrecio) || nuevoPrecio <= 0)
		return false;
	p->setPrecio(nuevoPrecio);
	return true;
}

bool GestorProductos::modificarNombre(const std::string& cod, const std::string& nuevoNombre) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr || !p->getActivo() || !tieneTexto(nuevoNombre))
		return false;
	p->setNombre(nuevoNombre);
	return true;
}

bool GestorProductos::ingresaMerca(const std::string& cod, int cantidad) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr || !p->getActivo())
		return false;
	return p->agregarExistencia(cantidad);
}

bool GestorProductos::desactivar(const std::string& cod) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr || !p->getActivo())
		return false;
	p->desactivar();
	return true;
}

void GestorProductos::setProductos(const std::vector<Producto>& lista) {
	productos = lista;
}
