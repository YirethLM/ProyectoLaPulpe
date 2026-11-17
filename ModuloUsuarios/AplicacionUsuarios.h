
#pragma once
#include <functional>

class QWidget;

int ejecutarLaPulpe(
    const std::function<QWidget* ()>& crearProductos
);

