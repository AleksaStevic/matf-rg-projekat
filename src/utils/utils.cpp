#include <rg/utils/utils.hpp>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace rg {
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
    }
}