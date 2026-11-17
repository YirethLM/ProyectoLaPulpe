
#pragma once
#include "GeneradorTicket.h"

//Clase que genera comprobantes en formato HTML:joz1303
class TicketHtml : public GeneradorTicket {
public:
    string generar(const Venta& venta) const override;
    string extension() const override;
};
