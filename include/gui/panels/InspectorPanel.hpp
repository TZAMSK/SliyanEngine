#pragma once

class Gui;
class Scene;
class ViewportRenderer;
class SelectionManager;

void drawInspectorPanel(Gui &gui, Scene &scene, ViewportRenderer &renderer, const SelectionManager &selection);
