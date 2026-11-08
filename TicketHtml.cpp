
#include "TicketHtml.h"
#include <sstream>
#include <iomanip>
using namespace std;

//Genera el contenido del recibo en HTML:joz1303
string TicketHtml::generar(const Venta& venta) const {

    ostringstream ticket;

    //Muestra los precios con dos decimales:joz1303
    ticket << fixed << setprecision(2);

    ticket << "<html><head><meta charset='UTF-8'></head><body>\n";
    ticket << "<h2>LA PULPE</h2>\n";
    ticket << "<p>Venta #" << venta.getId() << "</p>\n";
    ticket << "<p>Fecha: " << venta.getFecha() << "</p>\n";

    ticket << "<table border='1'>\n";
    ticket << "<tr><th>Cantidad</th><th>Producto</th><th>Subtotal</th></tr>\n";

    //Recorre los productos vendidos y crea una fila por cada uno:joz1303
    for (const LineaVenta& linea : venta.getLineas()) {
        ticket << "<tr>";
        ticket << "<td>" << linea.getCantidad() << "</td>";
        ticket << "<td>" << linea.getNombre() << "</td>";
        ticket << "<td>" << linea.getSubtotal() << "</td>";
        ticket << "</tr>\n";
    }

    ticket << "</table>\n";

    ticket << "<p>Total: " << venta.getTotal() << "</p>\n";
    ticket << "<p>Pagado: " << venta.getPagado() << "</p>\n";
    ticket << "<p>Cambio: " << venta.getCambio() << "</p>\n";

    ticket << "</body></html>";

    return ticket.str(); //Devuelve todo el HTML generado como texto:joz1303
}

//Indica la extension del archivo generado:joz1303
string TicketHtml::extension() const {
    return ".html";
}