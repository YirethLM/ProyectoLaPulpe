#pragma once 
#include<string>
#include "GeneradorTicket.h"

class TicketTxt :public GeneradorTicket{//es la herencia de TicketTxt:joz1303
public:
	string generar(const Venta& venta) const override;
	string extension() const override; //rrellenando el metodo del molde".Si se equivoca en el nombre,Avisa:joz1303
};