#pragma once

#include <memory>
#include <utility>

#include "scene/shapes/Shape.hpp"

template <typename T, typename... Args> std::unique_ptr<T> createShape(Args &&...args)
{
    auto shape = std::make_unique<T>(std::forward<Args>(args)...);

    shape->updateMesh();

    return shape;
}
