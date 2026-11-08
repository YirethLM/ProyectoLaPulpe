
#include "AlmacenVentas.h"
#include <QFile>          //Permite abrir y manejar archivos:joz1303
#include <QJsonDocument>  //Convierte los datos a formato JSON:joz1303
#include <QJsonObject>    //Guarda los datos con nombre y valor:joz1303
#include <QJsonArray>     //Permite guardar listas dentro del JSON:joz1303

//Funcion para guardar todas las ventas en un archivo JSON:joz1303
bool AlmacenVentas::guardar(const string& ruta, const vector<Venta>& ventas) const {

    QJsonArray listaVentas; //Aqui guardaremos todas las ventas:joz1303

    //Recorremos una por una las ventas que tenemos:joz1303
    for (const Venta& venta : ventas) {

        QJsonObject datos; //Objeto que guarda la informacion de una venta:joz1303

        datos["id"] = venta.getId();
        datos["fecha"] = QString::fromStdString(venta.getFecha());
        datos["pagado"] = venta.getPagado();

        QJsonArray productos; //Lista de productos de esta venta:joz1303

        //Recorremos los productos que pertenecen a la venta:joz1303
        for (const LineaVenta& linea : venta.getLineas()) {

            QJsonObject producto; //Guarda los datos de un producto vendido:joz1303

            producto["codigo"] = QString::fromStdString(linea.getCodigo());
            producto["nombre"] = QString::fromStdString(linea.getNombre());
            producto["precioUsado"] = linea.getPrecioUsado();
            producto["cantidad"] = linea.getCantidad();

            productos.append(producto); //Agregamos el producto a la lista:joz1303
        }

        datos["lineas"] = productos; //Guardamos los productos dentro de la venta:joz1303
        listaVentas.append(datos);  //Agregamos la venta a la lista general:joz1303
    }

    QJsonObject raiz; //Objeto principal que contiene todas las ventas:joz1303
    raiz["ventas"] = listaVentas;

    QFile archivo(QString::fromStdString(ruta)); //Preparamos el archivo usando la ruta indicada:joz1303

    //Intentamos abrir el archivo para escribir los datos:joz1303
    if (!archivo.open(QIODevice::WriteOnly)) {
        return false; //Si no se puede abrir, avisamos que fallo:joz1303
    }

    //Convertimos los datos de ventas en texto JSON:joz1303
    QByteArray texto = QJsonDocument(raiz).toJson();

    //Escribimos el JSON y comprobamos que se guardo completo:joz1303
    bool correcto = archivo.write(texto) == texto.size();

    archivo.close(); //Cerramos el archivo despues de escribir:joz1303

    return correcto; //Devolvemos si se guardo correctamente:joz1303
}


//Funcion para recuperar las ventas que guardamos anteriormente:joz1303
bool AlmacenVentas::cargar(const string& ruta, vector<Venta>& ventas) const {

    QFile archivo(QString::fromStdString(ruta));

    //Abrimos el archivo para leer su contenido:joz1303
    if (!archivo.open(QIODevice::ReadOnly)) {
        return false; //Si no existe o no abre, devolvemos false:joz1303
    }

    //Leemos el contenido y lo convertimos a un documento JSON:joz1303
    QJsonDocument documento = QJsonDocument::fromJson(archivo.readAll());

    archivo.close(); //Ya leimos los datos, cerramos el archivo:joz1303

    //Comprobamos que el contenido tenga un objeto JSON principal:joz1303
    if (!documento.isObject()) return false;

    QJsonObject raiz = documento.object();

    //Verificamos que exista la lista de ventas:joz1303
    if (!raiz["ventas"].isArray()) return false;

    vector<Venta> lista; //Aqui guardaremos las ventas recuperadas:joz1303

    //Recorremos cada venta que encontramos en el JSON:joz1303
    for (const QJsonValue& dato : raiz["ventas"].toArray()) {

        if (!dato.isObject()) return false;

        QJsonObject objeto = dato.toObject();

        //Cada venta debe tener su lista de productos:joz1303
        if (!objeto["lineas"].isArray()) return false;

        //Creamos nuevamente el objeto Venta con los datos guardados:joz1303
        Venta venta(
            objeto["id"].toInt(),
            objeto["fecha"].toString().toStdString(),
            objeto["pagado"].toDouble()
        );

        //Recorremos los productos que pertenecian a esa venta:joz1303
        for (const QJsonValue& elemento : objeto["lineas"].toArray()) {

            if (!elemento.isObject()) return false;

            QJsonObject producto = elemento.toObject();

            //Reconstruimos cada linea con el precio original de la compra:joz1303
            LineaVenta linea(
                producto["codigo"].toString().toStdString(),
                producto["nombre"].toString().toStdString(),
                producto["precioUsado"].toDouble(),
                producto["cantidad"].toInt()
            );

            venta.agregarLinea(linea); //Agregamos el producto a su venta:joz1303
        }

        lista.push_back(venta); //Guardamos la venta reconstruida en el vector:joz1303
    }

    ventas = lista; //Pasamos las ventas recuperadas al vector recibido:joz1303

    return true; //Indicamos que la lectura termino correctamente:joz1303
}
/*ESTO ESTA INCREIBLE DE APRENDER ATTENTAAMENTE LOS STUDENTS*/