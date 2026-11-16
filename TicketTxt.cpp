#include "TicketTxt.h"
#include <iomanip>
#include <sstream>

// Convierte los datos de la venta en texto legible para el comprobante :joz1303
string TicketTxt::generar(const Venta& venta) const {
	ostringstream ticket; // Construye el contenido del ticket en memoria :joz1303
	ticket << fixed << setprecision(2); // Presenta los importes con dos decimales :joz1303
	ticket << "LA PULPE\n";
	ticket << "Venta #" << venta.getId() << " | Fecha: " << venta.getFecha() << "\n";
	ticket << "=====================================\n";

	// Recorre los productos vendidos y escribe una línea por cada producto :joz1303
	for (const LineaVenta& linea : venta.getLineas()) {
		ticket << linea.getCantidad() << " x " << linea.getNombre()
			<< " (" << linea.getCodigo() << ")\n";
		ticket << "  Precio unitario: " << linea.getPrecioUsado()
			<< " | Subtotal: " << linea.getSubtotal() << "\n";
	}

	ticket << "=====================================\n";
	// Resume total, monto recibido y cambio calculado :joz1303
	ticket << "Total: " << venta.getTotal() << "\n";
	ticket << "Pagado: " << venta.getPagado() << "\n";
	ticket << "Cambio: " << venta.getCambio() << "\n";
	return ticket.str();
}

string TicketTxt::extension() const {
	return ".txt";
}