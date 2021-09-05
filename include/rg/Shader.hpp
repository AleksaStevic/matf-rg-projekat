#ifndef MATF_RG_PROJEKAT_SHADER_H
#define MATF_RG_PROJEKAT_SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <rg/light.hpp>

namespace rg {
    class Shader {
        unsigned int pId;
    public:
        Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

        // activate the shader
        void use() const;

        // utility uniform functions
        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

        void setVec2(const std::string &name, const glm::vec2 &value) const;

        void setVec2(const std::string &name, float x, float y) const;

        void setVec3(const std::string &name, const glm::vec3 &value) const;

        void setVec3(const std::string &name, float x, float y, float z) const;

        void setVec4(const std::string &name, const glm::vec4 &value) const;

        void setVec4(const std::string &name, float x, float y, float z, float w) const;

        void setMat2(const std::string &name, const glm::mat2 &mat) const;

        void setMat3(const std::string &name, const glm::mat3 &mat) const;

        void setMat4(const std::string &name, const glm::mat4 &mat) const;

        void setLight(const std::string &name, const PointLight &light) const;

        void setLight(const std::string &name, const DirLight &light) const;

        void setLight(const std::string &name, const SpotLight &light) const;

        void deleteProgram();

    private:
        /**
         * Compile shader.
         *
         * @param type The type of shader we want to compile: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
         * @param source The source of the shader as a string.
         * @return Shader ID.
         */
        static int compileShader(GLenum type, const std::string &source);
    };
}


#endif //MATF_RG_PROJEKAT_SHADER_H
