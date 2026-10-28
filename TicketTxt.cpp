#include "TicketTxt.h"
#include <string>
#include <sstream>

string TicketTxt::generar(const Venta& venta) const {
	ostringstream ticket; //se usa ostringstream para construir 
	//el contenido del ticket de manera eficient en ves de usar cout/ este lo guarde en la variable:joz1303
	ticket << "LA PULPE-Pacman\n";
		ticket << "Venta #" << venta.getId() << " " << venta.getFecha() << "\n";

	ticket << "=====================================\n";
	for (const LineaVenta& linea : venta.getLineas()) { //rrecorremos todas las lineas de la venta y escribe una por producto:joz1303
		ticket <<linea.getCantidad()<<"x"<<linea.getNombre()
			<< " " << linea.getSubtotal() << "\n";
	}

	ticket << "=====================================\n";
	ticket << "Total: " << venta.getTotal() << "\n";
	ticket << "Pagado: " << venta.getPagado() << "\n";
	ticket << "Cambio: " << venta.getCambio() << "\n";
	return ticket.str();

}
string TicketTxt::extension() const {
	return ".txt";
}