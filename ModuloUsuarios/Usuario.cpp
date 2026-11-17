
#include "Usuario.h"

Usuario::Usuario(
    int i,
    const std::string& n,
    const std::string& u,
    const std::string& h,
    Rol r,
    bool a)
    : id(i),
    nombre(n),
    nombreUsuario(u),
    hashContrasena(h),
    rol(r),
    activo(a) {
}

int Usuario::obtenerId() const {
    return id;
}

std::string Usuario::obtenerNombre() const {
    return nombre;
}

std::string Usuario::obtenerNombreUsuario() const {
    return nombreUsuario;
}

const std::string& Usuario::obtenerHashContrasena() const {
    return hashContrasena;
}

Rol Usuario::obtenerRol() const {
    return rol;
}

bool Usuario::estaActivo() const {
    return activo;
}
