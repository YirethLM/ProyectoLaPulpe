#include "TicketHtml.h"
#include <iomanip>
#include <sstream>
using namespace std;

namespace {
// Evita que nombres de productos alteren el marcado HTML del comprobante :joz1303
string escaparHtml(const string& texto) {
    string resultado;
    resultado.reserve(texto.size());
    // Sustituye caracteres reservados por entidades HTML seguras :joz1303
    for (const char caracter : texto) {
        switch (caracter) {
        case '&': resultado += "&amp;"; break;
        case '<': resultado += "&lt;"; break;
        case '>': resultado += "&gt;"; break;
        case '"': resultado += "&quot;"; break;
        case '\'': resultado += "&#39;"; break;
        default: resultado += caracter; break;
        }
    }
    return resultado;
}
}

// Genera un comprobante HTML con los datos originales de la venta :joz1303
string TicketHtml::generar(const Venta& venta) const {
    ostringstream ticket; // Arma el documento HTML antes de escribir el archivo :joz1303
    ticket << fixed << setprecision(2); // Muestra los precios con dos decimales :joz1303
    ticket << "<html><head><meta charset='UTF-8'></head><body>\n";
    ticket << "<h2>LA PULPE</h2>\n";
    ticket << "<p>Venta #" << venta.getId() << "</p>\n";
    ticket << "<p>Fecha: " << escaparHtml(venta.getFecha()) << "</p>\n";
    ticket << "<table border='1'>\n";
    ticket << "<tr><th>Cantidad</th><th>Producto</th><th>Código</th>"
        << "<th>Precio unitario</th><th>Subtotal</th></tr>\n";

    // Recorre los productos vendidos y crea una fila por cada uno :joz1303
    for (const LineaVenta& linea : venta.getLineas()) {
        ticket << "<tr><td>" << linea.getCantidad() << "</td>";
        ticket << "<td>" << escaparHtml(linea.getNombre()) << "</td>";
        ticket << "<td>" << escaparHtml(linea.getCodigo()) << "</td>";
        ticket << "<td>" << linea.getPrecioUsado() << "</td>";
        ticket << "<td>" << linea.getSubtotal() << "</td></tr>\n";
    }

    ticket << "</table>\n";
    // Añade los valores finales calculados a partir de las líneas de venta :joz1303
    ticket << "<p>Total: " << venta.getTotal() << "</p>\n";
    ticket << "<p>Pagado: " << venta.getPagado() << "</p>\n";
    ticket << "<p>Cambio: " << venta.getCambio() << "</p>\n";
    ticket << "</body></html>";
    return ticket.str(); // Devuelve todo el HTML generado como texto :joz1303
}

// Indica la extensión del archivo generado :joz1303
string TicketHtml::extension() const {
    return ".html";
}