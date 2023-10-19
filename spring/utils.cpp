#include "utils.h"

float ScalarModAngle(float Angle)
{
    // Note: The modulo is performed with unsigned math only to work
    // around a precision error on numbers that are close to PI

    // Normalize the range from 0.0f to XM_2PI
    Angle = Angle + M_PI;
    // Perform the modulo, unsigned
    float fTemp = fabsf(Angle);
    fTemp = fTemp - (M_2PI * static_cast<float>(static_cast<int>(fTemp / M_2PI)));
    // Restore the number to the range of -XM_PI to XM_PI-epsilon
    fTemp = fTemp - M_PI;
    // If the modulo'd value was negative, restore negation
    if (Angle < 0.0f) {
        fTemp = -fTemp;
    }
    return fTemp;
}

glm::vec4 Vec3TransformCoord(glm::mat4 mat, glm::vec3 vec)
{
    glm::vec4 vec4 = { vec, 0.0 };
    glm::vec4 res = mat * vec4;
    return res / res.w;
}
