#pragma once
#include <string>

class Producto {
private:
	std::string codigo;
	std::string nombre;
	double precio;
	int existencia;
	bool activo;

public:
	Producto(std::string codigo, std::string nombre, double precio, int existencia);

	std::string getCodigo() const;
	std::string getNombre() const;
	double getPrecio() const;
	int getExistencia() const;
	bool getActivo() const;

	void agregarExistencia(int cantidad);
	bool rebajarExistencia(int cantidad);
	void desactivar();
	void setPrecio(double nuevoPrecio);
	void setNombre(const std::string& nuevoNombre);
};
