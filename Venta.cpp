#include "Venta.h"  //23/10/26
using namespace std;
LineaVenta::LineaVenta(string codigo,string nombre),double precioUsado, int cantidad)
	:codigo(codigo), nombre(nombre), precioUsado(precioUsado), cantidad(cantidad){}
	//constructor de la linea de venta
	//inicializa los atributos con los valores proporcionados:joz1303

	string LineaVenta::getCodigo() const { return codigo; }
	string LineaVenta::getNombre() const { return nombre; }
	double LineaVenta::getPrecioUsado() const { return precioUsado; }
	int LineaVenta::getCantidad() const { return cantidad; }
	double LineaVenta::getSubtotal() const { return precioUsado * cantidad; }// el subtotal de la linea de venta:joz1303

	Venta::Venta(int id, string fecha, double pagado)
	:id(id),fecha(fecha),pagado(pagado){}

	void Venta::agregarLinea(const LineaVenta& linea) {
		lineas.push_back(linea); //agrega una linea de ventas al vector de lineas de la venta:joz1303

	}

	int Venta::getId() const { return id; }
	string Venta::getFecha() const { return fecha; }
	const vector<LineaVenta>& Venta::getLineas() const { return lineas; }
	double Venta::getPagado() const { return pagado; }

	double Venta::getTotal() const {
		double total = 0.0;
		for (const auto& linea : lineas) {
			total += linea.getSubtotal(); //suma los subtotales de todas las lineas de venta:joz1303
		}
		return total;
	}

	double Venta::getCambio() const {
		return pagado - getTotal(); //calcula el cambio a devolver al cliente:joz1303
	}