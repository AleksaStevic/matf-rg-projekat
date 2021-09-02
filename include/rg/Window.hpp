//
// Created by aleksastevic on 8/31/21.
//

#ifndef MATF_RG_PROJEKAT_WINDOW_HPP
#define MATF_RG_PROJEKAT_WINDOW_HPP

#include <string>
#include <glm/glm.hpp>

namespace rg {
    class Window {
        GLFWwindow *window;
        int width, height;
        bool firstMouse = true;
        float lastX, lastY; // For mouse offset
        float lastFrame = glfwGetTime();
        float deltaTime;
    public:
        Window(int width, int height, const std::string &name);

        GLFWwindow *ptr() const;

        void setAsCurrentContext() const;

        void setFramebufferSizeCallback(GLFWframebuffersizefun cb);

        void setKeyCallback(GLFWkeyfun cb);

        void setCursorPositionCallback(GLFWcursorposfun cb);

        void setScrollCallback(GLFWscrollfun cb);

        bool keyPressed(int key) const;

        bool shouldClose() const;
//        void setShouldClose(bool shouldClose);

        void swapBuffers() const;

        int getWidth() const;

        int getHeight() const;

        void setWidth(int w);

        void setHeight(int h);

        glm::vec2 getMouseOffset(float mouseX, float mouseY);

        void updateDeltaTime();

        float getDeltaTime() const;
    };
}

#endif //MATF_RG_PROJEKAT_WINDOW_HPP
