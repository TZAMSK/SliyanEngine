#pragma once

enum class ViewMode
{
    Solid,
    Wireframe
};

struct ViewportSettings
{
    ViewMode viewMode = ViewMode::Solid;
};
