# Pruebas del módulo de productos

Estas pruebas revisan agregar, buscar, modificar, ingresar y rebajar existencias, y desactivar productos, incluyendo datos incorrectos.

Con un compilador compatible con C++17, ejecutar desde la raíz del repositorio:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic LaPulpe_productos/Producto.cpp LaPulpe_productos/GestorProductos.cpp pruebas/prueba_productos.cpp -o prueba_productos
./prueba_productos
```

En Windows, ejecutar `prueba_productos.exe`.

Estas pruebas no requieren Qt, pero **no sustituyen** las pruebas de la ventana gráfica ni las de lectura/escritura de JSON, que requieren Qt.
