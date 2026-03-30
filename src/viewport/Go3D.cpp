#include "viewport/Go3d.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void introduce3D(unsigned int shaderProgram, float width, float height)
{
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::lookAt(
        // Position
        glm::vec3(6.0f, -6.0f, 6.0f),
        // Look-at
        glm::vec3(0.0f, 0.0f, 0.0f),
        //"Sky"
        glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "proj");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}
