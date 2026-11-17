#include "Producto.h"
#include <limits>
#include <cmath>

Producto::Producto(std::string codigo, std::string nombre, double precio, int existencia)
	: codigo(codigo), nombre(nombre), precio(precio), existencia(existencia), activo(true) {
}

std::string Producto::getCodigo() const { return codigo; }
std::string Producto::getNombre() const { return nombre; }
double Producto::getPrecio() const { return precio; }
int Producto::getExistencia() const { return existencia; }
bool Producto::getActivo() const { return activo; }

bool Producto::agregarExistencia(int cantidad) {
	if (!activo || cantidad <= 0 || cantidad > std::numeric_limits<int>::max() - existencia)
		return false;
	existencia += cantidad;
	return true;
}

void Producto::setPrecio(double nuevoPrecio) {
	if (activo && std::isfinite(nuevoPrecio) && nuevoPrecio > 0) precio = nuevoPrecio;
}

void Producto::setNombre(const std::string& nuevoNombre) {
	if (activo && !nuevoNombre.empty()) nombre = nuevoNombre;
}

bool Producto::rebajarExistencia(int cantidad) {
	if (!activo || cantidad <= 0 || cantidad > existencia) return false;
	existencia -= cantidad;
	return true;
}

void Producto::desactivar() { activo = false; }
