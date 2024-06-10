#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.h"
#include "mat.h"

glm::mat4 OrbitCamera::ViewMatrix() const noexcept
{
    auto position = direction * (distance * zoom);
    return Mat::translation({ 0.0, 0.0, -distance })* Mat::rotationX(pitch)* Mat::rotationY(yaw)* Mat::translation(-center);
}

glm::vec3 OrbitCamera::CameraPosition() const noexcept
{
    if (distance == 0.0f)
        return center;
    glm::mat4 view_mtx = ViewMatrix();
    glm::mat4 inv_view_mtx = glm::inverse(view_mtx);
    glm::vec3 res{ 0.0, 0.0, 0.0 };
    auto e = Vec3TransformCoord(inv_view_mtx, res);
    return vec3(e.x, e.y, e.z);
}

void OrbitCamera::SetTarget(glm::vec3 t) noexcept
{
    center = t;
    needUpdate = true;
}

void OrbitCamera::MoveTarget(glm::vec3 delta_target) noexcept
{
    center += delta_target;
    needUpdate = true;
}

void OrbitCamera::ObjectRotation(float dyaw, float dpitch) noexcept
{
    yaw += dyaw;
    pitch += dpitch;
	pitch = glm::clamp(pitch, -glm::half_pi<float>() * 0.99f, glm::half_pi<float>() * 0.99f);
    UpdateCameraVectors();
    needUpdate = true;
}

void OrbitCamera::UpdateCameraVectors() noexcept
{
    direction = {
        cos(pitch) * cos(yaw),
        sin(pitch),
        cos(pitch) * sin(yaw),
    };
    direction = glm::normalize(direction);
    right = glm::normalize(glm::cross(worldUp, direction));
    up = glm::normalize(glm::cross(direction, right));
}

void FpsCamera::MoveTarget(Camera_Movement action, float deltaTime) noexcept
{
    vec3 step{};
    switch (action)
    {
    case FORWARD:
        step = -Forward() * deltaTime * MOVEMENT_SPEED;
        break;
    case BACKWARD:
        step = Forward() * deltaTime * MOVEMENT_SPEED;
        break;
    case LEFT:
        step = -Right() * deltaTime * MOVEMENT_SPEED;
        break;
    case RIGHT:
        step = Right() * deltaTime * MOVEMENT_SPEED;
        break;
    case UP:
        step = vec3(0,1,0) * deltaTime * MOVEMENT_SPEED;
        break;
    case DOWN:
        step = vec3(0, -1, 0) * deltaTime * MOVEMENT_SPEED;
        break;
    default:
        break;
    }
    MoveTarget(step);
}

vec3 FpsCamera::Forward() const noexcept
{
    return glm::normalize(Mat::rotationY(-Angle_y()) * vec4(0, 0, 1, 0));
}

vec3 FpsCamera::Right() const noexcept
{
    return glm::normalize(Mat::rotationY(-Angle_y()) * vec4(1, 0, 0, 0));
}