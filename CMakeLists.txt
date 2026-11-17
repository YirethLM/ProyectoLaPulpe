
cmake_minimum_required(VERSION 3.21)

project(LaPulpeUsuarios LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.12.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

file(GLOB FUENTES_USUARIOS CONFIGURE_DEPENDS
    "ModuloUsuarios/*.cpp"
)

add_executable(PruebaUsuarios
    main_prueba.cpp
    ${FUENTES_USUARIOS}
)

target_link_libraries(PruebaUsuarios PRIVATE
    Qt6::Widgets
    Qt6::Network
)

if(WIN32)
    add_custom_command(TARGET PruebaUsuarios POST_BUILD
        COMMAND "${CMAKE_PREFIX_PATH}/bin/windeployqt.exe"
            "$<TARGET_FILE:PruebaUsuarios>"
        COMMENT "Copiando las bibliotecas de Qt"
    )
endif()


if(MSVC)
    target_compile_options(PruebaUsuarios PRIVATE /utf-8)
endif()
