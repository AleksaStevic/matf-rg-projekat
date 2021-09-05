//
// Created by aleksastevic on 9/3/21.
//

#ifndef MATF_RG_PROJEKAT_MESH_HPP
#define MATF_RG_PROJEKAT_MESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include <rg/Shader.hpp>

namespace rg {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;

        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture {
        unsigned int id;
        std::string type; // texture_diffuse, texture_specular, texture_normal, texture_height
        std::string path;
    };

    class Mesh {
        unsigned int VAO{};
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::string glslIdentifierPrefix;

        Mesh(std::vector<Vertex> vs, std::vector<unsigned int> ind,
             std::vector<Texture> tex);

        void draw(Shader &shader);

    private:
        void setupMesh();
    };
}

#endif //MATF_RG_PROJEKAT_MESH_HPP
