#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <rg/Shader.hpp>
#include <rg/utils/debug.hpp>
#include <utility>

namespace rg {
    Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {
//        appendShaderFolderIfNotPresent(vertexShaderPath);
//        appendShaderFolderIfNotPresent(fragmentShaderPath);
        std::string vsString = readFileContents(std::move(vertexShaderPath));
        ASSERT(!vsString.empty(), "Vertex shader source is empty!");
        std::string fsString = readFileContents(std::move(fragmentShaderPath));
        ASSERT(!fsString.empty(), "Fragment shader source is empty!");

        int vertexShader = compileShader(GL_VERTEX_SHADER, vsString);
        int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsString);

        // Link Shaders:
        int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check for linking errors:
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog <<
                      std::endl;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        pId = shaderProgram;
    }

    // activate the shader
    void Shader::use() const {
        ASSERT(pId > 0, "Use of undefined or deleted program.");
        glUseProgram(pId);
    }

    // utility uniform functions
    void Shader::setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(pId, name.c_str()), (int) value);
    }

    void Shader::setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(pId, name.c_str()), value);
    }

    void Shader::setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(pId, name.c_str()), value);
    }

    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(pId, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(pId, name.c_str()), x, y);
    }

    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(pId, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(pId, name.c_str()), x, y, z);
    }

    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(pId, name.c_str()), 1, &value[0]);
    }

    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(pId, name.c_str()), x, y, z, w);
    }

    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(pId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(pId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(pId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::deleteProgram() {
        glDeleteProgram(pId);
        pId = 0;
    }

    int Shader::compileShader(GLenum type, const std::string &source) {
        const char *shaderSource = source.c_str();
        int shaderId = glCreateShader(type);
        glShaderSource(shaderId, 1, &shaderSource, nullptr);
        glCompileShader(shaderId);

        // Check for compilation errors.
        int success;
        char infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                      << "::COMPILATION_FAILED\n"
                      << infoLog <<
                      std::endl;
        }

        return shaderId;
    }

}