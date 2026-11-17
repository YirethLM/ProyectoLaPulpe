# Ejecutar La Pulpe

La carpeta `LaPulpe_productos` contiene las clases del módulo. El archivo `main.cpp` inicia la ventana y `productos.json` almacena los datos.

## Compilar la ventana (requiere Qt Widgets y CMake)

```bash
cmake -S . -B build
cmake --build build
```

Ejecutar el programa desde la carpeta raíz del repositorio para que encuentre `productos.json`. Para realizar una demostración, conviene trabajar sobre una copia de ese archivo, porque las operaciones actualizan su contenido.

## Ejecutar pruebas sin Qt

```bash
cmake -S . -B build-pruebas -DCONSTRUIR_VENTANA=OFF
cmake --build build-pruebas
ctest --test-dir build-pruebas --output-on-failure
```

La ventana y el almacenamiento JSON deben verificarse también con Qt instalado.
