#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <rg/Shader.hpp>
#include <rg/Texture2D.hpp>
#include <rg/Window.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, __attribute__((unused)) int scancode, int action, int mods);

void update(GLFWwindow *window);

int main() {
    rg::Window::glfwInit(3, 3, GLFW_OPENGL_CORE_PROFILE);
    rg::Window window(800, 600, "Hello Window");
    window.setAsCurrentContext();
    window.setFramebufferSizeCallback(framebufferSizeCallback);
    window.setKeyCallback(keyCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    rg::Shader shader("resources/shaders/vertexShader.vs", "resources/shaders/fragmentShader.fs");

    float vertices[] = {
            // first triangle
            0.5f, 0.5f, 0.0f, 1.0, 1.0, //leva
            0.5f, -0.5f, 0.0f, 1.0, 0.0,  // desna
            -0.5f, -0.5f, 0.0f, 0.0, 0.0, // desna
            -0.5f, 0.5f, 0.0f, 0.0, 1.0 // gore
    };

    unsigned indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    // Vertex Buffer Object
    // Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    while (!window.shouldClose()) {
        glfwPollEvents();
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        texture.activate();
        texture.bind();

        texture1.activate();
        texture1.bind();

//        int uniformId = glGetUniformLocation(shaderProgram, "gColor");
        shader.use();
//        glUniform4f(uniformId, sin(glfwGetTime()) / 2 + 0.5, 0.0, 0.0, 1.0);

        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        update(window);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        window.swapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.deleteProgram();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void update(GLFWwindow *window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//        glfwSetWindowShouldClose(window, true);
//    }

//    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
//        std::cerr << "G" << std::endl;
//    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
//
//    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
//        glClearColor(1.0, 0.0, 0.0, 1.0);
//    }
//
//    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
//        glClearColor(0.0, 1.0, 0.0, 1.0);
//    }
//
//    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
//        glClearColor(0.0, 0.0, 1.0, 1.0);
//    }
}