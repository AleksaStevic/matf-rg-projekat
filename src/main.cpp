#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <rg/Shader.hpp>
#include <rg/Model.hpp>
#include <rg/Window.hpp>
#include <rg/Camera.hpp>
#include <rg/utils/utils.hpp>
#include <rg/light.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void update(const std::unique_ptr<rg::Window> &window);

std::unique_ptr<rg::Window> window;
rg::Camera camera;

int main() {
    rg::glfwInit(3, 3, GLFW_OPENGL_CORE_PROFILE);
    window = std::make_unique<rg::Window>(1280, 720, "Hello Window");
    window->setAsCurrentContext();
    window->setFramebufferSizeCallback(framebufferSizeCallback);
    window->setKeyCallback(keyCallback);
    window->setCursorPositionCallback(mouseCallback);
    window->setScrollCallback(scrollCallback);

    glfwSetInputMode(window->ptr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    rg::loadGlad();
    stbi_set_flip_vertically_on_load(true);

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->ptr(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glEnable(GL_DEPTH_TEST);

    rg::Shader shader("resources/shaders/vertexShader.vs", "resources/shaders/fragmentShader.fs");
    rg::Model model("resources/objects/backpack/backpack.obj");
    model.setTextureNamePrefix("material.");

    rg::PointLight pointLight{
            glm::vec3(4.0f, 4.0f, 0.0f),
            glm::vec3(0.4, 0.4, 0.2),
            glm::vec3(0.6, 0.5, 0.6),
            glm::vec3(1.0f),
            1.0f,
            0.09f,
            0.032f
    };

    while (!window->shouldClose()) {

        glfwPollEvents();

        window->updateDeltaTime();
        update(window);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        pointLight.position = glm::vec3(4.0f * cos(glfwGetTime()), 4.0f, 4.0f * sin(glfwGetTime()));
//        shader.setVec3("pointLight.position", pointLight.position);
//        shader.setVec3("pointLight.ambient", pointLight.ambient);
//        shader.setVec3("pointLight.specular", pointLight.specular);
//        shader.setVec3("pointLight.diffuse", pointLight.diffuse);
//        shader.setFloat("pointLight.constant", pointLight.constant);
//        shader.setFloat("pointLight.linear", pointLight.linear);
//        shader.setFloat("pointLight.quadratic", pointLight.quadratic);
        shader.setLight("pointLight", pointLight);
        shader.setFloat("material.shininess", 32.0f);
        shader.setVec3("viewPosition", camera.position);

        float windowAspectRatio = (float) window->getWidth() / (float) window->getHeight();
        glm::mat4 projection = camera.getPerspectiveMatrix(windowAspectRatio);
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        m = glm::scale(m, glm::vec3(1.0f, 1.0f, 1.0f));    // it's a bit too big for our scene, so scale it down
        shader.setMat4("model", m);
        model.draw(shader);

        {
            ImGui::Begin("Text Window");
            ImGui::Text("Hello World!");
            ImGui::End();
        }

        // ImGui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->swapBuffers();
        glfwPollEvents();
    }

    // ImGui CleanUp
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *w, int width, int height) {
    glViewport(0, 0, width, height);
    window->setWidth(width);
    window->setHeight(height);
}

void update(const std::unique_ptr<rg::Window> &window) {

    const float deltaTime = window->getDeltaTime();

    if (window->keyPressed(GLFW_KEY_W)) {
        camera.move(rg::FORWARD, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_S)) {
        camera.move(rg::BACKWARD, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_D)) {
        camera.move(rg::RIGHT, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_A)) {
        camera.move(rg::LEFT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    glm::vec2 mouseOffset = window->getMouseOffset(xPos, yPos);
    camera.rotate(mouseOffset.x, mouseOffset.y, true);
}

void scrollCallback(GLFWwindow *w, double xOffset, double yOffset) {
    camera.zoom(yOffset);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}