#include <cmath>
#include <memory>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <rg/Shader.hpp>
#include <rg/Model.hpp>
#include <rg/Camera.hpp>
#include <rg/utils/utils.hpp>
#include <rg/light.hpp>
#include <rg/utils/textures.hpp>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void drawImGui();

void update(GLFWwindow *window);

glm::vec3 spotLightAmbient = glm::vec3(0.0f);
glm::vec3 spotLightDiffuse = glm::vec3(1.0f);
glm::vec3 spotLightSpecular = glm::vec3(10.0f, 5.f, 10.f);

rg::SpotLight spotLight{
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        spotLightAmbient,
        spotLightDiffuse,
        spotLightSpecular,
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(15.0f)),
        1.0f,
        0.02f,
        0.005f
};

rg::PointLight pointLight{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.01),
        glm::vec3(10.0),
        glm::vec3(10.0f, 10.0f, 10.0f),
        0.5f,
        0.01f,
        0.001f
};

bool hdr = true;
bool bloom = true;
bool spotLightEnabled = true;
float exposure = 1.0f;
int numberOfAsteroids = 50;
int effect = 0;

constexpr float PI = glm::radians(360.f);

glm::vec3 sunPosition{0.0f};
glm::vec3 mercuryPosition{};
glm::vec3 earthPosition{};

float mercurySpeed = 0.05f;
float earthSpeed = 0.1f;
float cameraDefaultSpeed = 3.f;

rg::Camera camera{glm::vec3(0.0f, 0.0f, 10.0f)};

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

    // GLFW Config
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

    float asteroidVertices[] = {
            0.5, 0.0, -0.5, 0.0, 0.0, 0.5, 0.5, 0.0,
            0.5, 0.0, 0.5, 0.0, 1.0, 0.5, 0.5, 0.0,
            0.0, 0.5, 0.0, 0.5, 0.5, 0.5, 0.5, 0.0,

            0.5, 0, 0.5, 0, 0, 0, 0.5, 0.5,
            -0.5, 0, 0.5, 0, 1, 0, 0.5, 0.5,
            0, 0.5, 0, 0.5, 0.5, 0, 0.5, 0.5,

            -0.5, 0, 0.5, 0, 0, -0.5, 0.5, 0,
            -0.5, 0, -0.5, 0, 1, -0.5, 0.5, 0,
            0, 0.5, 0, 0.5, 0.5, -0.5, 0.5, 0,

            0.5, 0, -0.5, 0, 1, 0, 0.5, -0.5,
            -0.5, 0, -0.5, 0, 0, 0, 0.5, -0.5,
            0, 0.5, 0, 0.5, 0.5, 0, 0.5, -0.5,

            0.5, 0, -0.5, 0, 0, 0.5, -0.5, 0,
            0.5, 0, 0.5, 0, 1, 0.5, -0.5, 0,
            0, -0.5, 0, 0.5, 0.5, 0.5, -0.5, 0,

            0.5, 0, 0.5, 0, 0, 0, -0.5, 0.5,
            -0.5, 0, 0.5, 0, 1, 0, -0.5, 0.5,
            0, -0.5, 0, 0.5, 0.5, 0, -0.5, 0.5,

            -0.5, 0, 0.5, 0, 0, -0.5, -0.5, 0,
            -0.5, 0, -0.5, 0, 1, -0.5, -0.5, 0,
            0, -0.5, 0, 0.5, 0.5, -0.5, -0.5, 0,

            0.5, 0, -0.5, 0, 1, 0, -0.5, -0.5,
            -0.5, 0, -0.5, 0, 0, 0, -0.5, -0.5,
            0, -0.5, 0, 0.5, 0.5, 0, -0.5, -0.5,
    };

    unsigned int asteroidIndices[] = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23
    };

    std::vector<glm::vec3> asteroidRotations(numberOfAsteroids);
    for (auto &v: asteroidRotations) {
        v = rg::randomVec3(-1.0f, 1.0f);
    }

    std::vector<std::pair<float, float>> asteroidValues(numberOfAsteroids);
    for (auto &v: asteroidValues) {
        float rRadius = rg::random(-2.f, 2.f) + 30.0f;
        float ry = rg::random(-2.f, 2.f);
        v.first = rRadius;
        v.second = ry;
    }

    unsigned int asteroidVAO, asteroidVBO, asteroidEBO;
    glGenVertexArrays(1, &asteroidVAO);
    glGenBuffers(1, &asteroidVBO);
    glGenBuffers(1, &asteroidEBO);
    glBindVertexArray(asteroidVAO);
    glBindBuffer(GL_ARRAY_BUFFER, asteroidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(asteroidVertices), asteroidVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asteroidEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(asteroidIndices), asteroidIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);
//    glBindVertexArray(0);

    float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; ++i) {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, attachments);

    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer not completed.");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int pingpongFBO[2];
    unsigned int pingpongColorBuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorBuffers);
    for (unsigned int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorBuffers[i], 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int screenFBO;
    unsigned int screenColorBuffer;
    glGenFramebuffers(1, &screenFBO);
    glGenTextures(1, &screenColorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
    glBindTexture(GL_TEXTURE_2D, screenColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenColorBuffer, 0);

    std::vector<std::string> faces{
            "resources/textures/cubemaps/space/right.jpg",
            "resources/textures/cubemaps/space/left.jpg",
            "resources/textures/cubemaps/space/up.jpg",
            "resources/textures/cubemaps/space/down.jpg",
            "resources/textures/cubemaps/space/front.jpg",
            "resources/textures/cubemaps/space/back.jpg"
    };
    unsigned int cubemapTexture = rg::loadCubemap(faces, false, true);
    unsigned int blackWood = rg::loadTexture("resources/textures/black_wood.jpg", true, true);
    unsigned int blackWoodSpecular = rg::loadTexture("resources/textures/black_wood_specular.jpg", true, true);

    // Shaders and models and lights.
    rg::Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    rg::Shader planetShader("resources/shaders/planet.vs", "resources/shaders/planet.fs");
    rg::Shader sunShader("resources/shaders/sun.vs", "resources/shaders/sun.fs");
    rg::Shader hdrShader("resources/shaders/hdr.vs", "resources/shaders/hdr.fs");
    rg::Shader blurShader("resources/shaders/blur.vs", "resources/shaders/blur.fs");
    rg::Shader asteroidShader("resources/shaders/asteroid.vs", "resources/shaders/asteroid.fs");
    rg::Shader screenShader("resources/shaders/screen.vs", "resources/shaders/screen.fs");

    rg::Model earth("resources/objects/earth/scene.gltf", true);
    rg::Model sun("resources/objects/sun/Sun.obj");
    rg::Model mercury("resources/objects/mercury_planet/scene.gltf", true);
//    rg::Model asteroid("resources/objects/asteroid/scene.gltf", true);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    blurShader.use();
    blurShader.setInt("image", 0);

    hdrShader.use();
    hdrShader.setInt("scene", 0);
    hdrShader.setInt("bloomBlur", 1);

    asteroidShader.use();
    asteroidShader.setInt("diffuseMap", 0);
    asteroidShader.setInt("specularMap", 1);

    screenShader.use();
    screenShader.setInt("screenTexture", 0);
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
//        glClearColor(programState->clearColor.r, programState->clearColor.g, programState->clearColor.b, 1.0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Render
        glm::mat4 projection = camera.getPerspectiveMatrix((float) windowWidth / (float) windowHeight);
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        spotLight.position = camera.position;
        spotLight.direction = camera.front;

        // Setup Shaders
        planetShader.use();
        planetShader.setLight("pointLight", pointLight);
        planetShader.setLight("spotLight", spotLight);
        planetShader.setVec3("viewPos", camera.position);
        planetShader.setMat4("projection", projection);
        planetShader.setMat4("view", view);

        asteroidShader.use();
        asteroidShader.setLight("pointLight", pointLight);
        asteroidShader.setLight("spotLight", spotLight);
        asteroidShader.setVec3("viewPos", camera.position);
        asteroidShader.setMat4("projection", projection);
        asteroidShader.setMat4("view", view);

        sunShader.use();
        sunShader.setMat4("projection", projection);
        sunShader.setMat4("view", view);

        mercuryPosition = sunPosition + glm::vec3(60.0f * sin(glfwGetTime() * mercurySpeed), 0.0f,
                                                  60.0f * cos(glfwGetTime() * mercurySpeed));

//        jupiterPosition = sunPosition + glm::vec3(0.0f, 0.0f, glfwGetTime());

        earthPosition = mercuryPosition + glm::vec3(20.0f * sin(glfwGetTime() * earthSpeed), 0.0f,
                                                    20.0f * cos(glfwGetTime() * earthSpeed));
        planetShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, mercuryPosition);
        planetShader.setMat4("model", model);
        mercury.draw(planetShader);

        planetShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, earthPosition);
        model = glm::scale(model, glm::vec3(1.5f));
        planetShader.setMat4("model", model);
        earth.draw(planetShader);

        asteroidShader.use();
        float interval = PI / numberOfAsteroids;
        for (int i = 0; i < numberOfAsteroids; ++i) {
            float rRadius, ry;
            std::tie(rRadius, ry) = asteroidValues[i];
            model = glm::mat4(1.0f);
            model = glm::translate(model,
                                   glm::vec3(rRadius * std::sin(i * interval + glfwGetTime() * 0.1f), ry,
                                             rRadius * std::cos(interval * i + glfwGetTime() * 0.1f)));
            model = glm::rotate(model, (float) glm::radians(glfwGetTime() * 20.f), asteroidRotations[i]);
//        model = glm::scale(model, glm::vec3(5.0f));
            asteroidShader.setMat4("model", model);
            glBindVertexArray(asteroidVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, blackWood);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, blackWoodSpecular);
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
//        glDrawArrays(GL_TRIANGLES, 0, 120);
            glBindVertexArray(0);
        }


        // Draw sun
        sunShader.use();
        model = glm::mat4(1.0f);
        sunShader.setMat4("model", model);
        sun.draw(sunShader);

        // Draw Skybox
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        bool horizontal = true;
        bool first_iteration = true;
        blurShader.use();
        unsigned int amount = 10;
        for (unsigned int i = 0; i < amount; ++i) {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            blurShader.setBool("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[i] : pingpongColorBuffers[!horizontal]);

            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorBuffers[!horizontal]);
        hdrShader.setBool("hdr", hdr);
        hdrShader.setBool("bloom", bloom);
        hdrShader.setFloat("exposure", exposure);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenShader.use();
        screenShader.setInt("effect", effect);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screenColorBuffer);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);


//        drawImGui();


        glfwSwapBuffers(window);
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
}

void update(GLFWwindow *window) {

    const float deltaTime = rg::getDeltaTime();

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(rg::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(rg::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(rg::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(rg::LEFT, deltaTime);
    }
}

void mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    glm::vec2 mouseOffset = rg::getMouseOffset((float) xPos, (float) yPos);
    camera.rotate(mouseOffset.x, mouseOffset.y, true);
}

void scrollCallback(GLFWwindow *w, double xOffset, double yOffset) {
    camera.movementSpeed += yOffset;
    if (camera.movementSpeed < cameraDefaultSpeed) {
        camera.movementSpeed = cameraDefaultSpeed;
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        hdr = !hdr;
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        bloom = !bloom;
    }

    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_3 && action == GLFW_PRESS) {
        effect = key - GLFW_KEY_0;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (spotLightEnabled) {
            spotLight.ambient = glm::vec3(0.0f);
            spotLight.specular = glm::vec3(0.0f);
            spotLight.diffuse = glm::vec3(0.0f);
            spotLightEnabled = false;
        } else {
            spotLight.ambient = spotLightAmbient;
            spotLight.specular = spotLightSpecular;
            spotLight.diffuse = spotLightDiffuse;
            spotLightEnabled = true;
        }
    }
}

void drawImGui() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Text Window");
        ImGui::Text("Hello World!");
        ImGui::End();
    }

    // ImGui render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}