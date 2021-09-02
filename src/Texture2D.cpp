//
// Created by aleksastevic on 8/31/21.
//

#include <glad/glad.h>

#include <rg/utils/debug.hpp>
#include <rg/Texture2D.hpp>

namespace rg {

    extern bool gladLoaded;

    Texture2D::Texture2D(const std::string &imgPath, GLint filtering, GLint sampling, GLint format, GLenum unit) : tId(
            0), unit(unit) {
        ASSERT(gladLoaded, "Glad is not loaded.");
        glGenTextures(1, &tId);
        glBindTexture(GL_TEXTURE_2D, tId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampling);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampling);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

        int width, height, nChannel;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(imgPath.c_str(), &width, &height, &nChannel, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            ASSERT(false, "Failed to load texture!\n");
        }

        stbi_image_free(data);
    }

    void Texture2D::activate() const {
        glActiveTexture(unit);
    }

    void Texture2D::bind() const {
        glBindTexture(GL_TEXTURE_2D, tId);
    }

    int Texture2D::getUnitOrder() const {
        switch (unit) {
            case GL_TEXTURE0:
                return 0;
            case GL_TEXTURE1:
                return 1;
            case GL_TEXTURE2:
                return 2;
            default:
                ASSERT(false, "Texture has unrecognized unit.");
        }
    }
}