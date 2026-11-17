# 🛒 LA PULPE
### Sistema de Gestión de Ventas e Inventario

**Curso:** Programación I  
**Profesor:** M.S. Alex Villegas Carranza  
**Periodo:** II Ciclo 2026

---

## 📖 Descripción del proyecto

**La Pulpe** es un sistema de gestión para una pulpería, desarrollado en C++ mediante programación orientada a objetos (POO) y Qt Widgets para la interfaz gráfica.

El objetivo es facilitar la administración de productos, el control de inventario, el registro de ventas, el proceso de cobro y la generación de comprobantes.

El proyecto se desarrolla colaborativamente, utilizando Git y GitHub para el control de versiones.

---

## ⚙️ Funcionalidades del sistema

El proyecto contempla las siguientes funcionalidades:

- Gestión y consulta de productos.
- Control y actualización de inventario.
- Registro de ventas y cálculo de totales.
- Procesamiento de pagos y cálculo del cambio.
- Consulta del historial de ventas.
- Búsqueda de ventas por identificador (ID).
- Generación de comprobantes en formato TXT.
- Generación de comprobantes en formato HTML.
- Almacenamiento de datos mediante archivos JSON.
- Navegación entre las diferentes áreas del sistema.

**Estado:** El proyecto se encuentra en fase de implementación e integración de sus módulos.

---

## 👥 Organización del equipo

El desarrollo se dividió en cinco áreas de trabajo:

| Área | Responsabilidad |
|------|-----------------|
| Área 1 | Productos e inventario |
| Área 2 | Ventas y cobro |
| Área 3 | Comprobantes e historial de ventas |
| Área 4 | Usuarios y roles |
| Área 5 | Ventana principal, navegación e integración |

Cada integrante trabaja en su área utilizando ramas de Git para mantener el proyecto organizado.

---

## 🧩 Área 3: Comprobantes e historial

El módulo de historial permite consultar las ventas registradas y buscar operaciones mediante su identificador.

Sus principales componentes son:

- `VentanaHistorial`: interfaz gráfica del historial.
- `GestorHistorial`: administración y consulta de registros.
- `AlmacenVentas`: persistencia de información.
- `Venta` y `LineaVenta`: representación de las ventas y sus productos.
- `GeneradorTicket`: estructura base de los comprobantes.
- `TicketTxt`: generación de comprobantes de texto.
- `TicketHtml`: generación de comprobantes HTML.

La interfaz fue desarrollada mediante Qt Designer y conectada con C++.

Este módulo cuenta con pruebas aisladas de compilación, búsqueda, almacenamiento y generación de comprobantes.

La integración con las ventas del Área 2 debe verificarse durante las pruebas generales del sistema.

---

## 💻 Tecnologías utilizadas

- Lenguaje de programación: C++.
- Interfaz gráfica: Qt Widgets y Qt Designer.
- Entorno de desarrollo: Microsoft Visual Studio.
- Control de versiones: Git y GitHub.
- Persistencia: archivos JSON.
- Modelado: diagramas UML.
- Prototipado: Figma.

---

## 🚀 Compilación y ejecución

### Requisitos

- Microsoft Visual Studio con soporte para C++.
- Qt instalado y configurado.
- Extensión Qt Visual Studio Tools, cuando corresponda.
- Acceso al código fuente del repositorio.

### Procedimiento general

1. Clonar o descargar el repositorio.
2. Abrir el proyecto correspondiente en Visual Studio.
3. Verificar la configuración de Qt y las dependencias.
4. Seleccionar una configuración de compilación compatible.
5. Compilar el proyecto.
6. Ejecutar la aplicación.

**Nota:** Los pasos definitivos de ejecución del sistema integrado se actualizarán al finalizar la integración de las cinco áreas.

---

## ⚡ Compilación con CMake (recomendada)

La carpeta `LaPulpe_productos` contiene las clases del módulo. El archivo `main.cpp` inicia la ventana y `productos.json` almacena los datos.

### Compilar la ventana (requiere Qt Widgets y CMake)

```bash
cmake -S . -B build
cmake --build build
```

Ejecutar el programa desde la carpeta raíz del repositorio para que encuentre `productos.json`. Para realizar una demostración, conviene trabajar sobre una copia de ese archivo, porque las operaciones actualizan su contenido.

### Ejecutar pruebas sin Qt

```bash
cmake -S . -B build-pruebas -DCONSTRUIR_VENTANA=OFF
cmake --build build-pruebas
ctest --test-dir build-pruebas --output-on-failure
```

La ventana y el almacenamiento JSON deben verificarse también con Qt instalado.

---

## 🧪 Pruebas

Se contemplan los siguientes casos:

- Registro y consulta de productos.
- Validación de operaciones de venta.
- Cálculo del total y del cambio.
- Consulta del historial de ventas.
- Búsqueda por ID.
- Lectura y escritura de archivos JSON.
- Generación de comprobantes TXT y HTML.
- Manejo de datos vacíos o incorrectos.
- Integración entre módulos.

Las evidencias y los resultados finales se incorporarán al informe del proyecto.

---

## 📂 Documentación y evidencias

### Primera parte: Análisis y diseño

**Diagrama UML:**  

**Prototipo interactivo de Figma:**  

**Documentación de la primera entrega:**  

### Segunda parte: Implementación

Las evidencias de implementación, pruebas, integración y defensa se incorporarán conforme avance el proyecto.

---

## 🌿 Metodología de trabajo

El equipo utiliza Git y GitHub para organizar el desarrollo mediante ramas y Pull Requests.

Las modificaciones se revisan antes de integrarlas a las ramas compartidas.

Se aplican principios de programación orientada a objetos, separación de responsabilidades y reutilización de código.

---

## 📚 Propósito académico

Este proyecto fue desarrollado como parte del curso Programación 1.

Su propósito es aplicar los conocimientos adquiridos en programación orientada a objetos, desarrollo de interfaces, persistencia de datos y trabajo colaborativo.
