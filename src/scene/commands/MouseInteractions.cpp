#include <glm/vec3.hpp>

bool detect_mouse_inside_shape(float posx, float posy, glm::vec3 coords)
{
    int numberOfPoints = glm::length(coords);
    float mousex = posx;
    float mousey = posy;

    int count = 0;

    for (int i = 0; i < numberOfPoints; i++)
    {
        glm::vec3 p1 = coords[0];
        glm::vec3 p2 = coords[(i + 1) % numberOfPoints];

        if ((mousey > std::min(p1.y, p2.y)) && (mousey <= std::max(p1.y, p2.y)) && (mousex <= std::max(p1.x, p2.x)))
        {
            double xIntersect = (mousey - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;

            if (p1.x == p2.x || mousex <= xIntersect)
            {
                count++;
            }
        }
    }

    return count % 2 == 1;
}
