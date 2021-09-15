#include <rg/utils/utils.hpp>

namespace rg {

    bool glfwInitialized = false;
    bool gladLoaded = false;
    bool firstMouse = true;
    float lastX{};
    float lastY{};
    float deltaTime{};
    float lastFrame{};

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

    void updateDeltaTime() {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    float getDeltaTime() {
        return deltaTime;
    }

    glm::vec2 getMouseOffset(float mouseX, float mouseY) {
        if (firstMouse) {
            lastX = mouseX;
            lastY = mouseY;
            firstMouse = false;
        }

        float xOffset = mouseX - lastX;
        float yOffset = lastY - mouseY;
        lastX = mouseX;
        lastY = mouseY;

        return {xOffset, yOffset};
    }

    void glfwInit(int majorVer, int minorVer, int profile) {
        ::glfwInit();
        // OpenGL 3.3 Core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVer);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVer);
        glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

        glfwInitialized = true;
    }

    GLFWwindow *createWindow(int width, int height, const char *name) {
        ASSERT(glfwInitialized, "GLFW is not initialized.");
        GLFWwindow *window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        ASSERT(window != nullptr, "Failed to create window.");

        return window;
    }
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
    os << v.x << ' ' << v.y << ' ' << v.z;
    return os;
}

std::istream &operator>>(std::istream &is, glm::vec3 &v) {
    is >> v.x >> v.y >> v.z;
    return is;
}