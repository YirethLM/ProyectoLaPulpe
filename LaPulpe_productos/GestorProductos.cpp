#include "GestorProductos.h"

bool GestorProductos::agregar(const Producto& nuevo) {
	if (nuevo.getCodigo().empty() || nuevo.getNombre().empty()) return false;
	if (nuevo.getPrecio() <= 0 || nuevo.getExistencia() < 0) return false;
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
	if (p == nullptr || nuevoPrecio <= 0)
		return false;
	p->setPrecio(nuevoPrecio);
	return true;
}

bool GestorProductos::modificarNombre(const std::string& cod, const std::string& nuevoNombre) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr || nuevoNombre.empty())
		return false;
	p->setNombre(nuevoNombre);
	return true;
}

bool GestorProductos::ingresaMerca(const std::string& cod, int cantidad) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr || cantidad <= 0)
		return false;
	p->agregarExistencia(cantidad);
	return true;
}

bool GestorProductos::desactivar(const std::string& cod) {
	Producto* p = buscarPorCod(cod);
	if (p == nullptr)
		return false;
	p->desactivar();
	return true;
}

void GestorProductos::setProductos(const std::vector<Producto>& lista) {
	productos = lista;
}
