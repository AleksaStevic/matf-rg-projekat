//
// Created by leksa on 17.9.21..
//

#ifndef MATF_RG_PROJEKAT_TEXTURES_HPP
#define MATF_RG_PROJEKAT_TEXTURES_HPP

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <stb_image.h>

#include <rg/utils/debug.hpp>

namespace rg {
    unsigned int loadTexture(char const *path, bool flip = false, bool gammaCorrection = false);

    unsigned int loadCubemap(const std::vector<std::string> &faces, bool flip = false, bool gammaCorrection = false);
}

#endif //MATF_RG_PROJEKAT_TEXTURES_HPP
