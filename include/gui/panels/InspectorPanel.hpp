#pragma once

class Gui;
class Scene;
class ViewportRenderer;
class SelectionManager;
class PhysicsSystem;

class Shape;

void drawInspectorPanel(Gui &gui, Scene &scene, ViewportRenderer &renderer, const SelectionManager &selection,
                        PhysicsSystem &physicsSystem);
void drawColorTab(Shape *sel);
void drawTransformTab(Shape *sel);
void drawPhysicsTab(Shape *sel, PhysicsSystem &physicsSystem);
void drawRelationsTab(Shape *sel);
