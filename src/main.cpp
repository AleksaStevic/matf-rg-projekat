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

struct ProgramState {
    bool imGuiEnabled = false;
    glm::vec3 clearColor{0.5f};
    rg::Camera camera{glm::vec3(0.0f, 0.0f, 10.0f)};
    glm::vec3 backpackPosition{0.0f, 0.0f, 0.0f};
    float backpackScale{1.0f};
    bool enableMouseCameraMovement = true;

    void loadFromDisk(std::string path);

    void saveToDisk(std::string path) const;
};

void ProgramState::saveToDisk(std::string path) const {
    std::ofstream out(path);
    out << imGuiEnabled << '\n'
        << clearColor << '\n'
        << camera.position << '\n';
}

void ProgramState::loadFromDisk(std::string path) {
    std::ifstream in(path);
    if (in) {
        in >> imGuiEnabled >> clearColor >> camera.position;
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void drawImGui(ProgramState *programState);

void update(const std::unique_ptr<rg::Window> &window);

std::unique_ptr<rg::Window> window;
ProgramState *programState;

int main() {


    rg::glfwInit(3, 3, GLFW_OPENGL_CORE_PROFILE);
    window = std::make_unique<rg::Window>(1280, 720, "Hello Window");
    window->setAsCurrentContext();
    window->setFramebufferSizeCallback(framebufferSizeCallback);
    window->setKeyCallback(keyCallback);
    window->setCursorPositionCallback(mouseCallback);
    window->setScrollCallback(scrollCallback);

    programState = new ProgramState();
    programState->loadFromDisk("resources/programState.txt");

    glfwSetInputMode(window->ptr(), GLFW_CURSOR,
                     programState->imGuiEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

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
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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


        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0);
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
        shader.setVec3("viewPosition", programState->camera.position);

        float windowAspectRatio = (float) window->getWidth() / (float) window->getHeight();
        glm::mat4 projection = programState->camera.getPerspectiveMatrix(windowAspectRatio);
        glm::mat4 view = programState->camera.getViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, programState->backpackPosition); // translate it down so it's at the center of the scene
        m = glm::scale(m,
                       glm::vec3(programState->backpackScale));    // it's a bit too big for our scene, so scale it down
        shader.setMat4("model", m);
        model.draw(shader);

        if (programState->imGuiEnabled)
            drawImGui(programState);


        window->swapBuffers();
        glfwPollEvents();
    }

    // ImGui CleanUp
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    programState->saveToDisk("resources/programState.txt");
    delete programState;
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
        programState->camera.move(rg::FORWARD, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_S)) {
        programState->camera.move(rg::BACKWARD, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_D)) {
        programState->camera.move(rg::RIGHT, deltaTime);
    }

    if (window->keyPressed(GLFW_KEY_A)) {
        programState->camera.move(rg::LEFT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    glm::vec2 mouseOffset = window->getMouseOffset(xPos, yPos);
    if (!programState->imGuiEnabled)
        programState->camera.rotate(mouseOffset.x, mouseOffset.y, true);
}

void scrollCallback(GLFWwindow *w, double xOffset, double yOffset) {
    programState->camera.zoom(yOffset);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        programState->imGuiEnabled = !programState->imGuiEnabled;
        if (programState->imGuiEnabled)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void drawImGui(ProgramState *programState) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Text Window");
        ImGui::Text("Hello World!");
        ImGui::DragFloat3("Backpack position", (float *) &programState->backpackPosition, 0.05f);
        ImGui::DragFloat("Backpack scale", &programState->backpackScale, 0.05f, 0.0, 4.0);
        ImGui::ColorEdit3("Background color", (float *) &programState->clearColor);
        ImGui::End();
    }

    // ImGui render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}