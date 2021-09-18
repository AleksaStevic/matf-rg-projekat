//
// Created by aleksastevic on 9/5/21.
//

#ifndef MATF_RG_PROJEKAT_LIGHT_HPP
#define MATF_RG_PROJEKAT_LIGHT_HPP

#include <glm/glm.hpp>

namespace rg {
    struct PointLight {
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    struct DirLight {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;
    };
}

#endif //MATF_RG_PROJEKAT_LIGHT_HPP
