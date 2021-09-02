#include <iostream>
#include <cstdlib>

#include <GLFW/glfw3.h>

#include <rg/Window.hpp>
#include <rg/utils/debug.hpp>

namespace rg {

    extern bool glfwInitialized;

    Window::Window(int width, int height, const std::string &name) : window(nullptr), width(width), height(height) {
        ASSERT(glfwInitialized, "GLFW is not initialized.");
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
        if (window == nullptr) {
            std::cout << "Failed to create window." << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
    }

    GLFWwindow *Window::ptr() const {
        return window;
    }

    void Window::setAsCurrentContext() const {
        glfwMakeContextCurrent(window);
    }

    void Window::setFramebufferSizeCallback(GLFWframebuffersizefun cb) {
        glfwSetFramebufferSizeCallback(window, cb);
    }

    void Window::setKeyCallback(GLFWkeyfun cb) {
        glfwSetKeyCallback(window, cb);
    }

    void Window::setCursorPositionCallback(GLFWcursorposfun cb) {
        glfwSetCursorPosCallback(window, cb);
    }

    void Window::setScrollCallback(GLFWscrollfun cb) {
        glfwSetScrollCallback(window, cb);
    }

    bool Window::keyPressed(int key) const {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void Window::swapBuffers() const {
        glfwSwapBuffers(window);
    }

    int Window::getWidth() const {
        return width;
    }

    int Window::getHeight() const {
        return height;
    }

    void Window::setWidth(int w) {
        width = w;
    }

    void Window::setHeight(int h) {
        height = h;
    }

    glm::vec2 Window::getMouseOffset(float mouseX, float mouseY) {
        if (firstMouse) {
            lastX = mouseX;
            lastY = mouseY;
            firstMouse = false;
        }

        float xOffset = mouseX - lastX;
        float yOffset = lastY - mouseY;
        lastX = mouseX;
        lastY = mouseY;

        return glm::vec2(xOffset, yOffset);
    }

    void Window::updateDeltaTime() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    float Window::getDeltaTime() const {
        return deltaTime;
    }
}