//
// Created by aleksastevic on 8/30/21.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/rg/utils/debug.hpp"

namespace rg {
    void glClearError() {
        while (glGetError() != GL_NO_ERROR) {}
    }

    const char *glErrorToString(GLenum error) {
        switch (error) {
            case GL_NO_ERROR:
                return "GL_NO_ERROR";
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "GL_INVALID_FRAMEBUFFER_OPERATION";
            case GL_OUT_OF_MEMORY:
                return "GL_OUT_OF_MEMORY";
            default: {
                std::cerr << "Unknown error." << std::endl;
                return "Unknown error.";
            }
        }
    }

    bool glLogCall(const char *file, int line, const char *call) {
        bool success = true;
        while (GLenum error = glGetError()) {
            std::cerr << "[OpenGL Error] " << error << '\n';
            std::cerr << "MSG: " << glErrorToString(error) << '\n';
            std::cerr << "File: " << file << '\n';
            std::cerr << "Line: " << line << '\n';
            std::cerr << "Call: " << call << '\n';
            success = false;
        }
        return success;
    }
}
