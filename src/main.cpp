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
#include <rg/utils/textures.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void drawImGui();

void update(GLFWwindow *window);

rg::ProgramState *programState;

bool blinnPhong = true;

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
    glfwSetInputMode(window, GLFW_CURSOR, programState->imGuiEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    // Load GLAD
    rg::loadGlad();

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

    float skyboxVertices[] = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    std::vector<std::string> faces{
            "resources/textures/cubemaps/space/right.jpg",
            "resources/textures/cubemaps/space/left.jpg",
            "resources/textures/cubemaps/space/up.jpg",
            "resources/textures/cubemaps/space/down.jpg",
            "resources/textures/cubemaps/space/front.jpg",
            "resources/textures/cubemaps/space/back.jpg"
    };
    unsigned int cubemapTexture = rg::loadCubemap(faces);
    unsigned int bumpTexture = rg::loadTexture("resources/objects/mars/Textures/Bump_2K.png");

    // Shaders and models and lights.
    rg::Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    rg::Shader shader("resources/shaders/planet.vs", "resources/shaders/planet.fs");
    rg::Model mars("resources/objects/mars/Mars 2K.obj");
//    rg::Model venus("resources/objects/venus/Venus_1K.obj");
    mars.setTextureNamePrefix("material.");
//    venus.setTextureNamePrefix("material.");
    rg::PointLight pointLight{
            glm::vec3(0.0f, 10.0f, 0.0f),
            glm::vec3(1.0),
            glm::vec3(1.0),
            glm::vec3(1.0f),
            1.0f,
            0.09f,
            0.032f
    };

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    // Loop
    while (!glfwWindowShouldClose(window)) {

        // Glfw
        int windowWidth, windowHeight;
        glfwPollEvents();
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Update Delta Time
        rg::updateDeltaTime();

        // Update Scene
        update(window);

        // OpenGL Clear
        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render
        glm::mat4 projection = programState->camera.getPerspectiveMatrix((float) windowWidth / (float) windowHeight);
        glm::mat4 view = programState->camera.getViewMatrix();

        // Draw Skybox
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);

        // Draw mars
        shader.use();
//        pointLight.position = glm::vec3(6.0f * cos(glfwGetTime()), 4.0f, 6.0f * sin(glfwGetTime()));
        shader.setLight("pointLight", pointLight);
        shader.setFloat("material.shininess", 1.0f);
        shader.setVec3("viewPosition", programState->camera.position);
        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, programState->backpackPosition);
//        model = glm::scale(model, glm::vec3(programState->backpackScale));
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        mars.draw(shader);

//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(5.0f, 0, 0.0f));
//        shader.use();
//        shader.setMat4("model", model);
//        venus.draw(shader);

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

    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        blinnPhong = !blinnPhong;
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