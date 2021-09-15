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

    GLFWwindow *createWindow(int width, int height, const std::string &name);

    void updateDeltaTime();

    float getDeltaTime();

    glm::vec2 getMouseOffset(float mouseX, float mouseY);

    float getWindowWidth();

    float getWindowHeight();

    template<class T>
    T clamp(const T &v, const T &lo, const T &hi) {
        return v < lo ? lo : v > hi ? hi : v;
    }
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v);

std::istream &operator>>(std::istream &is, glm::vec3 &v);

#endif //MATF_RG_PROJEKAT_UTILS_HPP
