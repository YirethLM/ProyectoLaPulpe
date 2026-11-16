#pragma once
#include <string>
#include <vector>
using namespace std;
//hacer la lina de venta para guardar el precio usado en el momento, asi ele historial no cambia 
//si cambia el producto. //Wiliam sigue durmiendo lol/22/10/26

class LineaVenta {
private: //atributos de la linea de venta
	string codigo;  
	string nombre;
	double precioUsado;
	int cantidad;



public:
	LineaVenta(string codigo, string nombre, double precioUsado, int cantidad);

	string getCodigo() const;
	string getNombre() const;
	double getPrecioUsado() const;
	int getCantidad() const;
	double getSubtotal() const;
};


//clase para lo que serian las ven
class Venta {
private:
	int id;
	string fecha;
	vector<LineaVenta>lineas;  // guarda las lineas dentro de la Venta, seria  la parte de la composicion:joz1303
	double pagado;            //sin esta venta no existe tal line:joz1303

public:
	Venta(int id, string fecha, double pagado);

	void agregarLinea(const LineaVenta& linea);

	int getId() const;  //get para leer atributos privados  tomando nota y usando el encapsolamiento esta nota la hice yo:Jjzo13
	string getFecha() const;
	const vector<LineaVenta>& getLineas() const; //pasamos la linea sin copiarla por el ampersand  y no permite cambiarla conts:joz1303

	double getPagado() const; //los const es para indicar que no se modifica el objeto:joz1303
	double getTotal() const;
	double getCambio() const;
	 //Lets'go Charizar 

};
