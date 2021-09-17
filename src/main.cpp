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
#include <rg/Camera.hpp>
#include <rg/ProgramState.hpp>
#include <rg/utils/utils.hpp>
#include <rg/light.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void drawImGui();

void update(GLFWwindow *window);

rg::ProgramState *programState;

int main() {
    // GLFW Init
    rg::glfwInit(3, 3, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow *window = rg::createWindow(1280, 720, "Hello Window");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Program State
//    programState = rg::ProgramState::loadFromDisk("resources/programState.txt");
    programState = new rg::ProgramState();

    // GLFW Config
//    glfwSetInputMode(window, GLFW_CURSOR, programState->imGuiEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    // Load GLAD
    rg::loadGlad();

    // STB Image Config
    stbi_set_flip_vertically_on_load(true);

    // ImGui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // OpenGL Config
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders and models and lights.
    rg::Shader shader("resources/shaders/vertexShader.vs", "resources/shaders/fragmentShader.fs");
    rg::Model model("resources/objects/earth/Earth 2K.obj");
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

    // Loop
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        rg::updateDeltaTime();
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

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        float windowAspectRatio = (float) windowWidth / (float) windowHeight;
        glm::mat4 projection = programState->camera.getPerspectiveMatrix(windowAspectRatio);
        glm::mat4 view = programState->camera.getViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        glm::mat4 m = glm::mat4(1.0f);
        m = glm::translate(m, programState->backpackPosition);
        m = glm::scale(m, glm::vec3(programState->backpackScale));
        shader.setMat4("model", m);
        model.draw(shader);

        if (programState->imGuiEnabled)
            drawImGui();


        glfwSwapBuffers(window);
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
}

void update(GLFWwindow *window) {

    const float deltaTime = rg::getDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        programState->camera.move(rg::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        programState->camera.move(rg::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        programState->camera.move(rg::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        programState->camera.move(rg::LEFT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    glm::vec2 mouseOffset = rg::getMouseOffset((float) xPos, (float) yPos);
    if (!programState->imGuiEnabled)
        programState->camera.rotate(mouseOffset.x, mouseOffset.y, true);
}

void scrollCallback(GLFWwindow *w, double xOffset, double yOffset) {
    programState->camera.zoom((float) yOffset);
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

void drawImGui() {

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