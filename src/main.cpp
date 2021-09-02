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
    window = std::make_unique<rg::Window>(800, 600, "Hello Window");
    window->setAsCurrentContext();
    window->setFramebufferSizeCallback(framebufferSizeCallback);
    window->setKeyCallback(keyCallback);
    window->setCursorPositionCallback(mouseCallback);
    window->setScrollCallback(scrollCallback);

    rg::loadGlad();

    glEnable(GL_DEPTH_TEST);

    rg::Shader shader("resources/shaders/vertexShader.vs", "resources/shaders/fragmentShader.fs");

    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    rg::Texture2D texture("resources/textures/container.jpg", GL_LINEAR, GL_REPEAT, GL_RGB, GL_TEXTURE0);
    rg::Texture2D texture1("resources/textures/awesomeface.png", GL_LINEAR, GL_REPEAT, GL_RGBA, GL_TEXTURE1);

    shader.use();
    shader.setInt("t0", texture.getUnitOrder());
    shader.setInt("t1", texture1.getUnitOrder());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!window->shouldClose()) {

        window->updateDeltaTime();

        glfwPollEvents();
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        texture.activate();
        texture.bind();

        texture1.activate();
        texture1.bind();

        // scale -> translat
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float) window->getWidth() / (float) window->getHeight(),
                                                0.1f, 100.0f);
//        int uniformId = glGetUniformLocation(shaderProgram, "gColor");
        shader.use();

        // camera:
        glm::mat4 view = camera.getViewMatrix();

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);

        for (unsigned i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::mat4(1.0f);
            float angle = 20.f * i;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0, 0.3, 0.5));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
//        glUniform4f(uniformId, sin(glfwGetTime()) / 2 + 0.5, 0.0, 0.0, 1.0);
        update(window);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        window->swapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
    shader.deleteProgram();
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