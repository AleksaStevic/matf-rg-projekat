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
        glm::vec3 clearColor{0.5f};
        rg::Camera camera{glm::vec3(0.0f, 0.0f, 10.0f)};
        glm::vec3 backpackPosition{0.0f, 0.0f, 0.0f};
        float backpackScale{1.0f};
        bool enableMouseCameraMovement = true;

        static ProgramState *loadFromDisk(const std::string &path);

        void saveToDisk(std::string path) const;
    };
}

#endif //MATF_RG_PROJEKAT_PROGRAMSTATE_HPP
