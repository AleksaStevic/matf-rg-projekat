#include <rg/Model.hpp>
#include <rg/utils/debug.hpp>
#include <stb_image.h>

namespace rg {

    Model::Model(const std::string &path, bool gammaCorrection) : gammaCorrection(gammaCorrection) {
        loadModel(path);
    }

    void Model::draw(Shader &shader) {
        for (Mesh &mesh: meshes) {
            mesh.draw(shader);
        }
    }

    void Model::loadModel(const std::string &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate |
                                                       aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                                                       aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            ASSERT(false, "Failed to load a model!");
        }
        this->directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode *node, const aiScene *scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex{};
            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            if (mesh->HasNormals()) {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }

            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;

                vertex.Tangent.x = mesh->mTangents[i].x;
                vertex.Tangent.y = mesh->mTangents[i].y;
                vertex.Tangent.z = mesh->mTangents[i].z;

                vertex.Bitangent.x = mesh->mBitangents[i].x;
                vertex.Bitangent.y = mesh->mBitangents[i].y;
                vertex.Bitangent.z = mesh->mBitangents[i].z;

            } else {
                vertex.TexCoords = glm::vec2(0.0);
            }
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        loadTextureMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);

        loadTextureMaterial(material, aiTextureType_SPECULAR, "texture_specular", textures);

        loadTextureMaterial(material, aiTextureType_NORMALS, "texture_normal", textures);

        loadTextureMaterial(material, aiTextureType_HEIGHT, "texture_height", textures);

        loadTextureMaterial(material, aiTextureType_AMBIENT, "texture_ambient", textures);

        loadTextureMaterial(material, aiTextureType_EMISSIVE, "texture_emissive", textures);

//        std::cout << material->GetName().C_Str() << std::endl;
//        for (unsigned i = 0; i < material->mNumProperties; ++i) {
//            auto property = material->mProperties[i];
//            auto type = property->mType;
//            std::cout << property->mKey.C_Str() << std::endl;
//        }


        return {vertices, indices, textures};
    }

    void Model::loadTextureMaterial(aiMaterial *mat, aiTextureType type, const std::string &typeName,
                                    std::vector<Texture> &textures) {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            auto it = loaded_textures.find(std::string(str.C_Str()));
            if (it != loaded_textures.end()) {
                textures.push_back(it->second);
                skip = true;
            }

            if (!skip) {
                Texture texture;
                texture.id = textureFromFile(str.C_Str());
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
//                loaded_textures.push_back(texture);
                loaded_textures[std::string(str.C_Str())] = texture;
            }
        }
    }

    void Model::setTextureNamePrefix(const std::string &prefix) {
        for (Mesh &mesh: meshes) {
            mesh.glslIdentifierPrefix = prefix;
        }
    }

    unsigned int Model::textureFromFile(const char *filename) const {
        std::string fullPath(directory + "/" + filename);

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrComponents, 0);
        ASSERT(data != nullptr, "Failed to load texture image");
        GLint internalFormat;
        GLint dataFormat;
        if (nrComponents == 1) {
            internalFormat = dataFormat = GL_RED;
        } else if (nrComponents == 3) {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        } else if (nrComponents == 4) {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        } else
            ASSERT(false,
                   "Unknown texture format at path: " << fullPath << " Number of components: " << nrComponents);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        return textureID;
    }
}