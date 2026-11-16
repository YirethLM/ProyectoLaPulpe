cmake_minimum_required(VERSION 3.16)
project(LaPulpe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CONSTRUIR_VENTANA "Compilar la ventana gráfica con Qt" ON)

add_library(productos
    LaPulpe_productos/Producto.cpp
    LaPulpe_productos/GestorProductos.cpp
)
target_include_directories(productos PUBLIC LaPulpe_productos)

if(CONSTRUIR_VENTANA)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    set(CMAKE_AUTOMOC ON)
    add_executable(LaPulpe
        main.cpp
        LaPulpe_productos/VentanaProductos.cpp
        LaPulpe_productos/AlmacenProductos.cpp
        LaPulpe_productos/VentanaProductos.h
        LaPulpe_productos/AlmacenProductos.h
    )
    target_link_libraries(LaPulpe PRIVATE productos Qt${QT_VERSION_MAJOR}::Widgets)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(prueba_productos pruebas/prueba_productos.cpp)
    target_link_libraries(prueba_productos PRIVATE productos)
    add_test(NAME productos COMMAND prueba_productos)
endif()
