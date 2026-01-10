#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

typedef glm::ivec2 iVec2;
typedef glm::ivec3 iVec3;
typedef glm::ivec4 iVec4;

CUSTOM_FORMAT(vec2, "X:{}, Y:{}", input.x, input.y);
CUSTOM_FORMAT(vec3, "X:{}, Y:{}, Z:{}", input.x, input.y, input.z);
CUSTOM_FORMAT(vec4, "X:{}, Y:{}, Z:{}, W:{}", input.x, input.y, input.w);

CUSTOM_FORMAT(iVec2, "X:{}, Y:{}", input.x, input.y);
CUSTOM_FORMAT(iVec3, "X:{}, Y:{}, Z:{}", input.x, input.y, input.z);
CUSTOM_FORMAT(iVec4, "X:{}, Y:{}, Z:{}, W:{}", input.x, input.y, input.w);