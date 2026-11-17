
#pragma once

#include "Usuario.h"
#include <QString>
#include <vector>
#include <memory>

class GestorUsuarios {
    std::vector<std::unique_ptr<Usuario>> usuarios;

    QString ruta;
    QString error;

    int siguienteId = 1;
    bool listo = false;

    bool guardar();
    bool autorizado(int actorId);

    bool datosValidos(
        const QString& nombre,
        const QString& usuario,
        const QString& clave,
        bool nueva
    );

    int administradoresActivos() const;

public:
    explicit GestorUsuarios(const QString& ruta);

    bool cargar();

    QString ultimoError() const;
    bool estaVacio() const;

    bool crearAdministradorInicial(
        const QString& nombre,
        const QString& usuario,
        const QString& clave
    );

    bool crearUsuario(
        int actorId,
        const QString& nombre,
        const QString& usuario,
        const QString& clave,
        Rol rol
    );

    bool modificarUsuario(
        int actorId,
        int id,
        const QString& nombre,
        Rol rol,
        const QString& nuevaClave = {}
    );

    bool cambiarEstado(
        int actorId,
        int id,
        bool activo
    );

    const Usuario* buscarUsuarioPorNombre(
        const QString& usuario
    ) const;

    const Usuario* buscarUsuarioPorId(int id) const;

    const std::vector<std::unique_ptr<Usuario>>&
        listarUsuarios() const;
};
