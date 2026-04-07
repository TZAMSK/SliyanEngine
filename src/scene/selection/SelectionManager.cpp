#include "scene/selection/SelectionManager.hpp"
#include "gui/Gui.hpp"
#include "render/ViewportRenderer.hpp"

#include <glad/glad.h>

void SelectionManager::updateHover(double mousePosX, double mousePosY, const Gui &gui, const ViewportRenderer &renderer)
{
    const ImVec2 viewportPos = gui.getViewportPos();
    const ImVec2 viewportSize = gui.getViewportSize();

    const float localX = static_cast<float>(mousePosX) - viewportPos.x;
    const float localY = static_cast<float>(mousePosY) - viewportPos.y;

    if (viewportSize.x <= 0.0f || viewportSize.y <= 0.0f)
    {
        hovered_id = no_object;
        return;
    }

    if (localX < 0.0f || localY < 0.0f || localX >= viewportSize.x || localY >= viewportSize.y)
    {
        hovered_id = no_object;
        return;
    }

    const float scaleX = static_cast<float>(renderer.getWidth()) / viewportSize.x;
    const float scaleY = static_cast<float>(renderer.getHeight()) / viewportSize.y;

    const int readX = static_cast<int>(localX * scaleX);
    const int readY = static_cast<int>((viewportSize.y - 1.0f - localY) * scaleY);

    unsigned int pickedID = 0;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer.getFbo());
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glReadPixels(readX, readY, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pickedID);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    hovered_id = pickedID;
}

void SelectionManager::clickSelect()
{
    selected_id = hovered_id;
}

void SelectionManager::clearSelection()
{
    selected_id = no_object;
}

unsigned int SelectionManager::hoveredId() const
{
    return hovered_id;
}

unsigned int SelectionManager::selectedId() const
{
    return selected_id;
}

bool SelectionManager::hasSelection() const
{
    return selected_id != no_object;
}

void SelectionManager::setSelected(unsigned int id)
{
    selected_id = id;
}
