//
// Created by aleksastevic on 8/31/21.
//

#ifndef MATF_RG_PROJEKAT_UTILS_HPP
#define MATF_RG_PROJEKAT_UTILS_HPP

#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <rg/utils/debug.hpp>

namespace rg {

    std::string readFileContents(std::string path);

    void loadGlad();

    void glfwInit(int majorVer, int minorVer, int profile);

    GLFWwindow *createWindow(int width, int height, const char *name);

    void updateDeltaTime();

    float getDeltaTime();

    glm::vec2 getMouseOffset(float mouseX, float mouseY);

    float getWindowWidth();

    float getWindowHeight();

    float clamp(float v, float lo, float hi);

    float map(float s, float a1, float a2, float b1, float b2);
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v);

std::istream &operator>>(std::istream &is, glm::vec3 &v);

#endif //MATF_RG_PROJEKAT_UTILS_HPP
