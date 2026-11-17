
#pragma once

#include "GestorUsuarios.h"

class Autenticacion {
    GestorUsuarios& gestor;

public:
    explicit Autenticacion(GestorUsuarios& gestor);

    const Usuario* iniciarSesion(
        const QString& usuario,
        const QString& contrasena
    ) const;

    bool tienePermiso(
        const Usuario& usuario,
        Rol requerido
    ) const;
};
