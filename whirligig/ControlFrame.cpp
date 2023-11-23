#include "ControlFrame.h"
#include <glm/gtx/euler_angles.hpp>

glm::mat4 ControlFrame::ModelMatrix()
{
    auto t = Mat::translation(glm::vec3(position[0], position[1], position[2]));
    auto r = glm::eulerAngleXYZ(euler_angles[0], euler_angles[1], euler_angles[2]);
    return t * r ;
}
