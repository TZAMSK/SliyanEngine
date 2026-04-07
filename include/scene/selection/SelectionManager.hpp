#pragma once

#include "gui/Gui.hpp"

#include <glad/glad.h>

class ViewportRenderer;

class SelectionManager
{
  public:
    void updateHover(double mousePosX, double mousePosY, const Gui &gui, const ViewportRenderer &renderer);

    void clickSelect();
    void clearSelection();

    unsigned int hoveredId() const;
    unsigned int selectedId() const;
    bool hasSelection() const;

    void setSelected(unsigned int id);

  private:
    static constexpr unsigned int no_object = 0;

    unsigned int hovered_id = no_object;
    unsigned int selected_id = no_object;
};
