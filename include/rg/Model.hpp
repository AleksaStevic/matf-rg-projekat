//
// Created by aleksastevic on 9/3/21.
//

#ifndef MATF_RG_PROJEKAT_MODEL_HPP
#define MATF_RG_PROJEKAT_MODEL_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <rg/Shader.hpp>
#include <rg/Mesh.hpp>

namespace rg {
    class Model {
    public:
        std::vector<Mesh> meshes;
        std::unordered_map<std::string, Texture> loaded_textures;

        std::string directory;
        bool gammaCorrection;

        explicit Model(const std::string &path, bool gammaCorrection = false);

        void draw(Shader &shader);

        void setTextureNamePrefix(const std::string &prefix);

    private:
        void loadModel(const std::string &path);

        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        void loadTextureMaterial(aiMaterial *mat, aiTextureType type, const std::string &typeName,
                                 std::vector<Texture> &textures);

        unsigned int textureFromFile(const char *filename) const;
    };
}

#endif //MATF_RG_PROJEKAT_MODEL_HPP
