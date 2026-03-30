#include "scene/commands/Shortcuts.hpp"
#include "app/Application.hpp"
#include "gui/Gui.hpp"

#include <GLFW/glfw3.h>

void openShortcut(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    (void)scancode;

    if (key == GLFW_KEY_A && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL))
    {
        Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
        if (app && app->getGui())
        {
            app->getGui()->setShowAddShape(true);
        }
    }
}
