//
// Created by aleksastevic on 9/3/21.
//

#ifndef MATF_RG_PROJEKAT_MODEL_HPP
#define MATF_RG_PROJEKAT_MODEL_HPP

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rg/Shader.hpp>
#include <rg/Mesh.hpp>

namespace rg {
    class Model {
    public:
        std::vector<Mesh> meshes;
        std::vector<Texture> loaded_textures;

        std::string directory;

        Model(std::string path);

        void Draw(Shader &shader);

    private:
        void loadModel(std::string path);

        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        void loadTextureMaterial(aiMaterial *mat, aiTextureType type, std::string typeName,
                                 std::vector<Texture> &textures);
    };

    unsigned int TextureFromFile(const char *filename, std::string directory);
}

#endif //MATF_RG_PROJEKAT_MODEL_HPP
