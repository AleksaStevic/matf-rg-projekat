#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rg/Shader.hpp>
#include <rg/Texture2D.hpp>
#include <rg/Window.hpp>
#include <rg/Camera.hpp>
#include <rg/utils/utils.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void update(const std::unique_ptr<rg::Window> &window);

std::unique_ptr<rg::Window> window;
rg::Camera camera;

int main() {
    rg::glfwInit(3, 3, GLFW_OPENGL_CORE_PROFILE);
    window = std::make_unique<rg::Window>(1280, 720, "Hello Window");
    window->setAsCurrentContext();
    window->setFramebufferSizeCallback(framebufferSizeCallback);
    window->setKeyCallback(keyCallback);
    window->setCursorPositionCallback(mouseCallback);
    window->setScrollCallback(scrollCallback);

    glfwSetInputMode(window->ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    rg::loadGlad();

    glEnable(GL_DEPTH_TEST);

    rg::Shader colorShader("resources/shaders/vertexShader.vs", "resources/shaders/fragmentShader.fs");
    rg::Shader lightShader("resources/shaders/lightVertexShader.vs", "resources/shaders/lightFragmentShader.fs");

    float vertices[] = {
            // positions          // normals           // diffuseMap coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };

//    unsigned indices[] = {
//            0, 1, 3,
//            1, 2, 3
//    };

    unsigned int VBO, VAO/*,EBO */;
    // Vertex Buffer Object
    // Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // coordinate
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // normale
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // coordinate tekstura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    rg::Texture2D diffuseMap("resources/textures/container2.png", GL_LINEAR, GL_REPEAT, GL_TEXTURE0);
    rg::Texture2D specularMap("resources/textures/container2_specular.png", GL_LINEAR, GL_REPEAT, GL_TEXTURE1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!window->shouldClose()) {

        window->updateDeltaTime();

        glfwPollEvents();
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        diffuseMap.activate();
        diffuseMap.bind();
        specularMap.activate();
        specularMap.bind();

        float windowAspectRatio = (float) window->getWidth() / (float) window->getHeight();
        glm::mat4 projection = camera.getPerspectiveMatrix(windowAspectRatio);
        glm::mat4 view = camera.getViewMatrix();

        colorShader.use();
        colorShader.setMat4("view", view);
        colorShader.setMat4("projection", projection);

        colorShader.setInt("material.diffuse", diffuseMap.getUnitOrder());
        colorShader.setInt("material.specular", specularMap.getUnitOrder());
        colorShader.setFloat("material.shininess", 32.0f);

        // directional light
        colorShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        colorShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        colorShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        colorShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        colorShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[0].constant", 1.0f);
        colorShader.setFloat("pointLights[0].linear", 0.09);
        colorShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        colorShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        colorShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[1].constant", 1.0f);
        colorShader.setFloat("pointLights[1].linear", 0.09);
        colorShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        colorShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        colorShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[2].constant", 1.0f);
        colorShader.setFloat("pointLights[2].linear", 0.09);
        colorShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        colorShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        colorShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        colorShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        colorShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("pointLights[3].constant", 1.0f);
        colorShader.setFloat("pointLights[3].linear", 0.09);
        colorShader.setFloat("pointLights[3].quadratic", 0.032);
        // spotLight
        colorShader.setVec3("spotLight.position", camera.position);
        colorShader.setVec3("spotLight.direction", camera.front);
        colorShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        colorShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        colorShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        colorShader.setFloat("spotLight.constant", 1.0f);
        colorShader.setFloat("spotLight.linear", 0.09);
        colorShader.setFloat("spotLight.quadratic", 0.032);
        colorShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        colorShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glBindVertexArray(VAO);

        for (unsigned i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::mat4(1.0f);
            float angle = 20.f * i;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0, 0.3, 0.5));
            colorShader.use();
            colorShader.setMat4("model", model);
//            colorShader.setVec3("viewPos", camera.position);
//            colorShader.setVec3("material.ambient", glm::vec3(0.1f, 0.5f, 0.31f));
//            colorShader.setVec3("material.diffuse", glm::vec3(0.2f, 0.6f, 0.31f));
//            colorShader.setVec3("material.specular", glm::vec3(0.8f));
//
//            colorShader.setVec3("light.position", camera.position);
//            colorShader.setVec3("light.direction", camera.front);
//            colorShader.setFloat("light.cutoff", glm::cos(glm::radians(12.0f)));
//            colorShader.setFloat("light.outerCutoff", glm::cos(glm::radians(16.0f)));
//            colorShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
//            colorShader.setVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
//            colorShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
//            colorShader.setFloat("light.constant", 1.0f);
//            colorShader.setFloat("light.linear", 0.02f);
//            colorShader.setFloat("light.quadratic", 0.03f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightShader.use();
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);

        for (auto &pointLightPosition: pointLightPositions) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPosition);
            model = glm::scale(model, glm::vec3(0.5f));
            lightShader.setMat4("model", model);
            lightShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        update(window);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        window->swapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
    colorShader.deleteProgram();
    lightShader.deleteProgram();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *w, int width, int height) {
    glViewport(0, 0, width, height);
    window->setWidth(width);
    window->setHeight(height);
}

void update(const std::unique_ptr<rg::Window> &window) {

    const float deltaTime = window->getDeltaTime();

    if (window->keyPressed(GLFW_KEY_W)) {
        camera.move(rg::FORWARD, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_S)) {
        camera.move(rg::BACKWARD, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_D)) {
        camera.move(rg::RIGHT, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_A)) {
        camera.move(rg::LEFT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    glm::vec2 mouseOffset = window->getMouseOffset(xPos, yPos);
    camera.rotate(mouseOffset.x, mouseOffset.y, true);
}

void scrollCallback(GLFWwindow *w, double xOffset, double yOffset) {
    camera.zoom(yOffset);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}