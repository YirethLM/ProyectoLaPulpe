
#pragma once
#include <QString>

namespace Clave {
    QString generar(const QString& texto);
    bool verificar(
        const QString& texto,
        const QString& registro
    );
    bool formatoValido(const QString& registro);
}

