//
// Created by aleksastevic on 8/31/21.
//

#ifndef MATF_RG_PROJEKAT_WINDOW_HPP
#define MATF_RG_PROJEKAT_WINDOW_HPP

#include <string>

namespace rg {
    class Window {
        GLFWwindow *window;
        int width, height;
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
    };
}

#endif //MATF_RG_PROJEKAT_WINDOW_HPP
