//
// Created by aleksastevic on 8/31/21.
//

#ifndef MATF_RG_PROJEKAT_WINDOW_HPP
#define MATF_RG_PROJEKAT_WINDOW_HPP

#include <string>

namespace rg {
    class Window {
        GLFWwindow *window;
        static bool glfwInitialized;
    public:
        Window(int width, int height, const std::string &name);

        GLFWwindow *ptr() const;

        void setAsCurrentContext() const;

        void setFramebufferSizeCallback(GLFWframebuffersizefun cb);

        void setKeyCallback(GLFWkeyfun cb);

        bool shouldClose() const;

        void swapBuffers() const;

        static void glfwInit(int majorVer, int minorVer, int profile);
    };
}

#endif //MATF_RG_PROJEKAT_WINDOW_HPP
