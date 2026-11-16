#include "Producto.h"

Producto::Producto(std::string codigo, std::string nombre, double precio, int existencia)
	: codigo(codigo), nombre(nombre), precio(precio), existencia(existencia), activo(true) {
}

std::string Producto::getCodigo() const { return codigo; }
std::string Producto::getNombre() const { return nombre; }
double Producto::getPrecio() const { return precio; }
int Producto::getExistencia() const { return existencia; }
bool Producto::getActivo() const { return activo; }

void Producto::agregarExistencia(int cantidad) {
	if (cantidad > 0) existencia += cantidad;
}

void Producto::setPrecio(double nuevoPrecio) {
	if (nuevoPrecio > 0) precio = nuevoPrecio;
}

void Producto::setNombre(const std::string& nuevoNombre) {
	if (!nuevoNombre.empty()) nombre = nuevoNombre;
}

bool Producto::rebajarExistencia(int cantidad) {
	if (cantidad <= 0 || cantidad > existencia) return false;
	existencia -= cantidad;
	return true;
}

void Producto::desactivar() { activo = false; }
