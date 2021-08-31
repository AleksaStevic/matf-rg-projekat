//
// Created by matf-rg on 30.10.20..
//

#ifndef MATF_RG_PROJEKAT_TEXTURE2D_H
#define MATF_RG_PROJEKAT_TEXTURE2D_H

#include <string>

#include <glad/glad.h>
#include <stb_image.h>
#include <rg/utils/debug.hpp>

namespace rg {
    class Texture2D {
        unsigned int tId;
        GLenum unit;
    public:
        Texture2D(const std::string &imgPath, GLint filtering, GLint sampling, GLint format, GLenum unit);

        void activate() const;

        void bind() const;

        int getUnitOrder() const;
    };
}

#endif //MATF_RG_PROJEKAT_TEXTURE2D_H
