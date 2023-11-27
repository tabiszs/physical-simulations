#include "Quaternion.h"
// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

// this implementation assumes normalized quaternion
// converts to Euler angles in 3-2-1 sequence
glm::vec3 Quaternion::ToEulerAngles(glm::vec4 q)
{
    glm::vec3 angles{};

    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
    angles.y = 2 * std::atan2(sinp, cosp) - M_PI / 2;

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = std::atan2(siny_cosp, cosy_cosp);

    return angles;
}

glm::vec4 Quaternion::ToQuaternion(glm::vec3 euler_angles) // roll (x), pitch (y), yaw (z), angles are in radians
{
    auto roll = euler_angles.x;
    auto pitch = -euler_angles.y;
    auto yaw = euler_angles.z;

    // Abbreviations for the various angular functions
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);

    glm::vec4 q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

glm::vec4 Quaternion::SLERP(const glm::vec4& q1, const glm::vec4& q2, float a)
{
    glm::vec4 q22 = q2;
    float cosTheta = glm::dot(q1, q2);

    // If cosTheta < 0, the interpolation will take the long way around the sphere.
    // To fix this, one quat must be negated.
    if (cosTheta < 0.0f)
    {
        q22 = -q2;
        cosTheta = -cosTheta;
    }

    // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
    if (cosTheta > 1.0f - FLT_EPSILON)
    {
        return LERP(q1, q2, a);
    }
    else
    {
        float angle = acos(cosTheta);
        return (q1 * sin((1.0f - a) * angle) + q2 * sin(a * angle)) / sin(angle);
    }

    return Quaternion::Normalize(q22);
}

glm::vec4 Quaternion::LERP(const glm::vec4& q1, const glm::vec4& q2, float a)
{
    glm::vec4 q{};
    q.x = q1.x * (1 - a) + q2.x * a;
    q.y = q1.y * (1 - a) + q2.y * a;
    q.z = q1.z * (1 - a) + q2.z * a;
    q.w = q1.w * (1 - a) + q2.w * a;
    return Quaternion::Normalize(q);
}

glm::vec4 Quaternion::Normalize(const glm::vec4& q)
{
    float q_magnitude = std::sqrt(glm::dot(q, q));
    return q / q_magnitude;
}

// https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation#cite_ref-10
glm::mat4 Quaternion::toMat4(const glm::vec4& q)
{
    float qr = q.w;
    float qi = q.x;
    float qj = q.y;
    float qk = q.z;

    // for unit quaternion - without normalization
    glm::mat4 m = glm::transpose(glm::mat4(
    1 - 2*(qj*qj + qk*qk), 2*(qi*qj - qk*qr), 2*(qi*qk + qj*qr), 0,
    2*(qi*qj + qk*qr), 1 - 2*(qi*qi + qk*qk), 2*(qj*qk - qi*qr), 0,
    2*(qi*qk - qj*qr), 2*(qj*qk + qi*qr), 1 - 2*(qi*qi + qj*qj), 0,
    0, 0, 0, 1
    ));

    return m;
}

