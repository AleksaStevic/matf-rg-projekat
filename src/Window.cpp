#include <iostream>
#include <cstdlib>

#include <GLFW/glfw3.h>

#include <rg/Window.hpp>
#include <rg/utils/debug.hpp>

namespace rg {
    Window::Window(int width, int height, const std::string &name) : window(nullptr) {
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

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(window);
    }

    void Window::swapBuffers() const {
        glfwSwapBuffers(window);
    }

    void Window::glfwInit(int majorVer, int minorVer, int profile) {
        ::glfwInit();
        // OpenGL 3.3 Core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVer);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVer);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

        glfwInitialized = true;
    }

    bool Window::glfwInitialized = false;
}