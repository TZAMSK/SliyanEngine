#pragma once

class Gui;
class Scene;
class ViewportRenderer;
class SelectionManager;

class Shape;

void drawInspectorPanel(Gui &gui, Scene &scene, ViewportRenderer &renderer, const SelectionManager &selection);
void drawColorTab(Shape *sel);
void drawTransformTab(Shape *sel);
void drawPhysicsTab();
