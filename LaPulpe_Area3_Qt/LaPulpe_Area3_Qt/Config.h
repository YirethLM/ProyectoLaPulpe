#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QFile>

class Config {
public:
    static QString getJsonPath(const QString& filename) {
        // Los archivos de datos se guardan junto al ejecutable.
        // No se debe buscar una carpeta fija llamada "laPulpe", porque
        // la ruta de salida de Visual Studio normalmente es x64\Debug o x64\Release.
        QDir dir(QCoreApplication::applicationDirPath());
        QString path = dir.filePath(filename);

        // Buscar también en los directorios superiores del proyecto.
        // Esto permite encontrar los JSON durante la ejecución desde x64\Debug.
        QDir buscador = dir;
        while (!QFileInfo::exists(path)) {
            if (!buscador.cdUp()) {
                break;
            }

            QString candidato = buscador.filePath(filename);
            if (QFileInfo::exists(candidato)) {
                path = candidato;
                break;
            }
        }

        // Crear los datos iniciales cuando se ejecuta por primera vez.
        if (!QFileInfo::exists(path) &&
            (filename == "usuarios.json" || filename == "productos.json" || filename == "ventas.json")) {
            QFile archivo(path);
            if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
                if (filename == "usuarios.json") {
                    archivo.write(R"([{"activo":true,"contrasena":"1234","nombre":"Administrador","rol":"Administrador","usuario":"admin"}])");
                } else if (filename == "productos.json") {
                    archivo.write("[]");
                } else {
                    archivo.write(R"({"ventas":[]})");
                }
                archivo.close();
            }
        }

        qDebug() << "Ruta JSON resuelta:" << path;
        return path;
    }
};

#endif // CONFIG_H
