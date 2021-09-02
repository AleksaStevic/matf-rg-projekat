#include <rg/utils/utils.hpp>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace rg {

    bool glfwInitialized = false;
    bool gladLoaded = false;

    std::string readFileContents(std::string path) {
        std::ifstream in(path);
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }

    void loadGlad() {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to init GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }

        gladLoaded = true;
    }

    void glfwInit(int majorVer, int minorVer, int profile) {
        ::glfwInit();
        // OpenGL 3.3 Core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVer);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVer);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

        glfwInitialized = true;
    }

    template<class T>
    T clamp(const T &v, const T &lo, const T &hi) {
        return v < lo ? lo : v > hi ? hi : v;
    }
}