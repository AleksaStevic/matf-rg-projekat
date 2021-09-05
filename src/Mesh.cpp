#include <rg/Mesh.hpp>
#include <rg/utils/debug.hpp>
#include <utility>

namespace rg {
    Mesh::Mesh(std::vector<Vertex> vs, std::vector<unsigned int> ind, std::vector<Texture> tex)
            : vertices(std::move(vs)), indices(std::move(ind)), textures(std::move(tex)) {
        setupMesh();
    }

    void Mesh::draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        for (unsigned int i = 0; i < textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string name = glslIdentifierPrefix;
            std::string number;
            name.append(textures[i].type);

            if (textures[i].type == "texture_diffuse") {
                number = std::to_string(diffuseNr++); // 1
            } else if (textures[i].type == "texture_specular") {
                number = std::to_string(specularNr++);
            } else if (textures[i].type == "texture_normal") {
                number = std::to_string(normalNr++);
            } else if (textures[i].type == "texture_height") {
                number = std::to_string(heightNr++);
            } else {
                ASSERT(false, "Unknown texture type");
            }

            name.append(number);
            shader.setInt(name, i); // texture_diffuse1
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::setupMesh() {

        unsigned int VBO;
        unsigned int EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, Position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, Normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, TexCoords)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, Tangent)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (offsetof(Vertex, Bitangent)));

        glBindVertexArray(0);
    }
}
