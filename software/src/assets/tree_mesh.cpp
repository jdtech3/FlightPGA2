#include "game/assets.hpp"

#include <glm/glm.hpp>

namespace assets{
    const Mesh tree_mesh = {
        {0, 0, 0},
        {
            {0.f, 0.f, constants::TREE_BUSH_HEIGHT+constants::TREE_TRUNK_HEIGHT},
            {-constants::TREE_BUSH_SIDE, +constants::TREE_BUSH_SIDE, constants::TREE_TRUNK_HEIGHT},
            {-constants::TREE_BUSH_SIDE, -constants::TREE_BUSH_SIDE, constants::TREE_TRUNK_HEIGHT},
            {+constants::TREE_BUSH_SIDE, -constants::TREE_BUSH_SIDE, constants::TREE_TRUNK_HEIGHT},
            {+constants::TREE_BUSH_SIDE, +constants::TREE_BUSH_SIDE, constants::TREE_TRUNK_HEIGHT},
            #ifdef TREE_TRUNK_ENABLE
                {-TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, constants::TREE_TRUNK_HEIGHT},
                {-TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, constants::TREE_TRUNK_HEIGHT},
                {+TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, constants::TREE_TRUNK_HEIGHT},
                {+TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, constants::TREE_TRUNK_HEIGHT},
                {-TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, 0.f},
                {-TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, 0.f},
                {+TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, 0.f},
                {+TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, 0.f}
            #endif
        },
        {
            // Tree Bush
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 1,
            // Tree Trunk
            #ifdef TREE_TRUNK_ENABLE
                5, 6, 9,
                6, 9, 10,
                6, 7, 10,
                7, 10, 11,
                11, 7, 12,
                7, 12, 8,
                8, 12, 5,
                12, 5, 9
            #endif
        },
        {
            // Tree Bush
            glm::normalize(glm::vec3(-constants::TREE_BUSH_HEIGHT, 0.f, constants::TREE_BUSH_SIDE)),
            glm::normalize(glm::vec3(0.f, -constants::TREE_BUSH_HEIGHT, constants::TREE_BUSH_SIDE)),
            glm::normalize(glm::vec3(constants::TREE_BUSH_HEIGHT, 0.f, constants::TREE_BUSH_SIDE)),
            glm::normalize(glm::vec3(0.f, constants::TREE_BUSH_HEIGHT, constants::TREE_BUSH_SIDE)),
            // Tree Trunk
            #ifdef TREE_TRUNK_ENABLE
                {-1.f, 0.f, 0.f},
                {-1.f, 0.f, 0.f},
                {0.f, -1.f, 0.f},
                {0.f, -1.f, 0.f},
                {1.f, 0.f, 0.f},
                {1.f, 0.f, 0.f},
                {0.f, 1.f, 0.f},
                {0.f, 1.f, 0.f}
            #endif
        },
        {
            // Tree Bush
            constants::TREE_BUSH_COLOR,
            constants::TREE_BUSH_COLOR,
            constants::TREE_BUSH_COLOR,
            constants::TREE_BUSH_COLOR,
            // Tree Trunk
            #ifdef TREE_TRUNK_ENABLE
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR,
                constants::TREE_TRUNK_COLOR
            #endif
        }
    };
}