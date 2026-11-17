
#pragma once
#include <string>

enum class Rol {
    Administrador,
    Cajero
};

class Usuario {
    int id;
    std::string nombre;
    std::string nombreUsuario;
    std::string hashContrasena;
    Rol rol;
    bool activo;

    friend class GestorUsuarios;

public:
    Usuario(int id,
        const std::string& nombre,
        const std::string& nombreUsuario,
        const std::string& hashContrasena,
        Rol rol,
        bool activo = true);

    int obtenerId() const;
    std::string obtenerNombre() const;
    std::string obtenerNombreUsuario() const;
    const std::string& obtenerHashContrasena() const;
    Rol obtenerRol() const;
    bool estaActivo() const;
};
