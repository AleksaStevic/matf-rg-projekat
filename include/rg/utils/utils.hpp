//
// Created by aleksastevic on 8/31/21.
//

#ifndef MATF_RG_PROJEKAT_UTILS_HPP
#define MATF_RG_PROJEKAT_UTILS_HPP

#include <string>

namespace rg {

    std::string readFileContents(std::string path);

    void loadGlad();

    void glfwInit(int majorVer, int minorVer, int profile);

    template<class T>
    T clamp(const T &v, const T &lo, const T &hi);
}

#endif //MATF_RG_PROJEKAT_UTILS_HPP
