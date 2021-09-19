//
// Created by leksa on 15.9.21..
//

#ifndef MATF_RG_PROJEKAT_PROGRAMSTATE_HPP
#define MATF_RG_PROJEKAT_PROGRAMSTATE_HPP

#include <string>

#include <glm/glm.hpp>

#include <rg/Camera.hpp>

namespace rg {
    struct ProgramState {
        bool imGuiEnabled = false;

        static ProgramState *loadFromDisk(const std::string &path);

        void saveToDisk(std::string path) const;
    };
}

#endif //MATF_RG_PROJEKAT_PROGRAMSTATE_HPP
