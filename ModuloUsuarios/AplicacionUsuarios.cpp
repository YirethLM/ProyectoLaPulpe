
#include "AplicacionUsuarios.h"
#include "VentanaLogin.h"
#include "VentanaUsuarios.h"

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <memory>

int ejecutarLaPulpe(
    const std::function<QWidget* ()>& crearProductos) {

    const QString carpeta =
        QStandardPaths::writableLocation(
            QStandardPaths::AppLocalDataLocation
        );

    if (carpeta.isEmpty()) {
        QMessageBox::critical(
            nullptr,
            "Error",
            "No se encontro una carpeta de datos de usuario."
        );
        return 1;
    }

    GestorUsuarios gestor(
        QDir(carpeta).filePath("usuarios.json")
    );

    if (!gestor.cargar()) {
        QMessageBox::critical(
            nullptr,
            "Usuarios",
            gestor.ultimoError()
        );
        return 1;
    }

    for (;;) {
        VentanaLogin login(gestor);

        if (login.exec() != QDialog::Accepted) {
            return 0;
        }

        const int id = login.obtenerIdUsuario();

        const auto u =
            gestor.buscarUsuarioPorId(id);

        if (!u || !u->estaActivo()) {
            continue;
        }

        QDialog inicio;
        inicio.setWindowTitle(
            "La Pulpe - Menu principal"
        );
        inicio.resize(440, 270);

        auto layout = new QVBoxLayout(&inicio);

        auto info = new QLabel(
            QString("Usuario: %1\nRol: %2")
            .arg(
                QString::fromStdString(
                    u->obtenerNombre()
                ),
                u->obtenerRol() == Rol::Administrador
                ? "Administrador"
                : "Cajero"
            )
        );

        layout->addWidget(info);

        if (u->obtenerRol() == Rol::Administrador) {
            auto productos = new QPushButton(
                "Productos e inventario"
            );

            auto usuarios = new QPushButton(
                "Gestionar usuarios"
            );

            layout->addWidget(productos);
            layout->addWidget(usuarios);

            QObject::connect(
                usuarios,
                &QPushButton::clicked,
                &inicio,
                [&] {
                    VentanaUsuarios ventana(
                        gestor, id, &inicio
                    );
                    ventana.exec();
                }
            );

            QObject::connect(
                productos,
                &QPushButton::clicked,
                &inicio,
                [&] {
                    const auto actual =
                        gestor.buscarUsuarioPorId(id);

                    if (!actual ||
                        !actual->estaActivo() ||
                        actual->obtenerRol() !=
                        Rol::Administrador) {
                        return;
                    }

                    std::unique_ptr<QWidget> ventana(
                        crearProductos
                        ? crearProductos()
                        : nullptr
                    );

                    if (!ventana) {
                        QMessageBox::information(
                            &inicio,
                            "Productos",
                            "Esta es la prueba del modulo "
                            "de usuarios. Use main.cpp para "
                            "conectar los productos existentes."
                        );
                        return;
                    }

                    QDialog contenedor(&inicio);

                    contenedor.setWindowTitle(
                        ventana->windowTitle()
                    );

                    contenedor.resize(800, 650);

                    auto contenido =
                        new QVBoxLayout(&contenedor);

                    contenido->addWidget(
                        ventana.release()
                    );

                    contenedor.exec();
                }
            );
        }
        else {
            auto aviso = new QLabel(
                "Sesion de cajero iniciada.\n"
                "Esta cuenta no puede administrar "
                "usuarios ni modificar inventario.\n"
                "El modulo de ventas se conecta "
                "por separado."
            );

            aviso->setWordWrap(true);
            layout->addWidget(aviso);
        }

        auto cerrarSesion = new QPushButton(
            "Cerrar sesion"
        );

        auto salir = new QPushButton("Salir");

        layout->addWidget(cerrarSesion);
        layout->addWidget(salir);

        QObject::connect(
            cerrarSesion,
            &QPushButton::clicked,
            &inicio,
            &QDialog::accept
        );

        QObject::connect(
            salir,
            &QPushButton::clicked,
            &inicio,
            &QDialog::reject
        );

        if (inicio.exec() != QDialog::Accepted) {
            return 0;
        }
    }
}
