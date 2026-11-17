#ifndef ESTILOS_H
#define ESTILOS_H

#include <QString>

namespace Estilos {
    const QString COLOR_PRIMARIO = "#8E24AA"; // Púrpura de las imágenes
    const QString COLOR_PRIMARIO_HOVER = "#A239BD";
    const QString COLOR_FONDO = "#F8F7F2";    // Fondo crema claro
    const QString COLOR_TARJETA = "#FFFFFF";
    const QString COLOR_TEXTO = "#1A1A1A";
    const QString COLOR_TEXTO_SECUNDARIO = "#666666";
    const QString COLOR_BORDE = "#E0E0E0";
    const QString COLOR_EXITO = "#4CAF50";
    const QString COLOR_ALERTA = "#FF9800";
    const QString COLOR_ERROR = "#F44336";

    const QString QSS_MAIN = 
        "QMainWindow { background-color: " + COLOR_FONDO + "; }"
        "QMenuBar { background-color: white; border-bottom: 1px solid " + COLOR_BORDE + "; padding: 5px; }"
        "QMenuBar::item { padding: 5px 15px; margin: 2px; border-radius: 4px; }"
        "QMenuBar::item:selected { background-color: #F0E6F2; color: " + COLOR_PRIMARIO + "; }"
        
        "QPushButton#btnCerrar { "
        "  background-color: transparent; color: " + COLOR_PRIMARIO + "; "
        "  border: 1px solid " + COLOR_PRIMARIO + "; border-radius: 8px; padding: 8px 16px; font-weight: bold; "
        "}"
        "QPushButton#btnCerrar:hover { background-color: #F3E5F5; }"

        "QPushButton#tarjetaBoton { "
        "  background-color: " + COLOR_TARJETA + "; border: 1px solid " + COLOR_BORDE + "; "
        "  border-radius: 12px; text-align: left; padding: 0px; "
        "}"
        "QPushButton#tarjetaBoton:hover { border: 1.5px solid " + COLOR_PRIMARIO + "; background-color: #FAFAFA; }"
        
        "QLabel#tituloTarjeta { font-size: 18px; font-weight: bold; color: #212121; }"
        "QLabel#descTarjeta { color: #757575; font-size: 14px; }"
        "QLabel#detalleTarjeta { color: " + COLOR_PRIMARIO + "; font-size: 12px; font-weight: bold; }"
        
        "QLabel#tituloSeccion { font-size: 28px; font-weight: bold; color: " + COLOR_TEXTO + "; }"
        "QLabel#subtituloSeccion { font-size: 16px; color: " + COLOR_TEXTO_SECUNDARIO + "; }"
        "QLabel#tagRol { "
        "  background-color: #E8F5E9; color: " + COLOR_EXITO + "; "
        "  padding: 4px 12px; border-radius: 12px; font-weight: bold; font-size: 12px; "
        "}"
        "QPushButton#btnAccion { "
        "  background-color: " + COLOR_PRIMARIO + "; color: white; border-radius: 8px; padding: 10px 20px; font-weight: bold; "
        "}";
}

#endif // ESTILOS_H
