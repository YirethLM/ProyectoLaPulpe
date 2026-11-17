cmake_minimum_required(VERSION 3.21)
project(LaPulpe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "C:/Qt/6.12.0/msvc2022_64")
endif()

option(CONSTRUIR_VENTANA "Compilar la ventana grafica con Qt" ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)
set(CMAKE_AUTOMOC ON)

file(GLOB FUENTES_USUARIOS CONFIGURE_DEPENDS
    "ModuloUsuarios/*.cpp"
)

add_library(productos
    LaPulpe_productos/Producto.cpp
    LaPulpe_productos/GestorProductos.cpp
)
target_include_directories(productos PUBLIC LaPulpe_productos)

if(CONSTRUIR_VENTANA)
    add_executable(LaPulpe
        main.cpp
        InicioLaPulpe.h
        VentanaVentas.cpp
        VentanaVentas.h
        ServicioVenta.cpp
        ServicioVenta.h
        LaPulpe_productos/VentanaProductos.cpp
        LaPulpe_productos/VentanaProductos.h
        LaPulpe_productos/AlmacenProductos.cpp
        LaPulpe_productos/AlmacenProductos.h
        ${FUENTES_USUARIOS}
    )
    target_link_libraries(LaPulpe PRIVATE productos Qt6::Widgets Qt6::Network)
endif()

add_executable(PruebaUsuarios
    main_prueba.cpp
    ${FUENTES_USUARIOS}
)
target_link_libraries(PruebaUsuarios PRIVATE Qt6::Widgets Qt6::Network)

if(WIN32)
    add_custom_command(TARGET PruebaUsuarios POST_BUILD
        COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe"
            "$<TARGET_FILE:PruebaUsuarios>"
        COMMENT "Copiando las bibliotecas de Qt"
    )
endif()

if(MSVC)
    target_compile_options(PruebaUsuarios PRIVATE /utf-8)
    if(CONSTRUIR_VENTANA)
        target_compile_options(LaPulpe PRIVATE /utf-8)
    endif()
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(prueba_productos pruebas/prueba_productos.cpp)
    target_link_libraries(prueba_productos PRIVATE productos)
    add_test(NAME productos COMMAND prueba_productos)
endif()
