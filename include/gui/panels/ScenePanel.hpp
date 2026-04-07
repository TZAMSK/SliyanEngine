#pragma once

class Scene;
class Shape;
class SelectionManager;

void drawScenePanel(Scene &scene, SelectionManager &selectionManager);
static void drawShapeNode(Scene &scene, Shape *shape, SelectionManager &selectionManager);
