#pragma once
#include <string>
#include <vector>
using namespace std;
//24/10/26

//abstract definicion QUe  it would will do a ticket, *no como* :Joz1303

class GeneradorTicket {
public:
	virtual  ~GeneradorTicket() {} 
	//destructor virtual para permitir la destruccion correcta de objetos derivados:joz1303

	virtual string generar(const Venta& venta) const = 0;
	string extenison() const = 0;  //virtual al llamar (generar) ejecuta la version de clase hija (polimorfismo) :joz1303

};

