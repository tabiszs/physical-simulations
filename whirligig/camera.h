#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

static constexpr float OBJECT_ROTATION_SPEED = 0.6f;
static constexpr float ROTATION_SPEED = 0.0015f;
static constexpr float ZOOM_SPEED = 0.5f;
static constexpr float MOVEMENT_SPEED = 1.0f;

using namespace glm;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

class Camera
{
public:
    // returns the view matrix calculated from translation, rotation and scale
    [[nodiscard]] virtual glm::mat4 ViewMatrix() const noexcept = 0;
    constexpr bool NeedUpdate() { return needUpdate; }
    constexpr void Updated() { needUpdate = false; }
protected:
    bool needUpdate = false;
};

class OrbitCamera: public Camera
{
public:
    constexpr void DistanceRange(float min_distance, float max_distance) noexcept
    {
        if (max_distance < min_distance)
            max_distance = min_distance;
        d_min = min_distance;
        d_max = max_distance;
        ClampDistance();
    }

    explicit constexpr OrbitCamera(
        glm::vec3 center = { 0.0, 0.0, 0.0 }, 
        float min_distance = 0.0f,
        float max_distance = std::numeric_limits<float>::max(), 
        float distance = 0.0f) noexcept
        : yaw{}, pitch{}, d_min{}, d_max{}, distance{distance}, center{ center.x, center.y, center.z}
    {
        DistanceRange(min_distance, max_distance);
        needUpdate = true;
    }

    explicit constexpr OrbitCamera(
        float min_distance, 
        float max_distance = std::numeric_limits<float>::max(),
        float distance = 0.0f) noexcept
        : OrbitCamera{ { 0.0f, 0.0f, 0.0f }, min_distance, max_distance, distance }
    { }
    [[nodiscard]] glm::mat4 ViewMatrix() const noexcept override;
    [[nodiscard]] glm::vec3 CameraPosition() const noexcept;

    void SetTarget(glm::vec3 target) noexcept;
    void MoveTarget(glm::vec3 delta_target) noexcept;
    void ObjectRotation(float dyaw, float dpitch) noexcept;
    constexpr void Zoom(float d_distance) noexcept
    {
        distance -= d_distance; // Zoom in mean getting smaller distance 
        ClampDistance();
        needUpdate = true;
    }

    [[nodiscard]] constexpr float Angle_y() const noexcept { return yaw; }
    [[nodiscard]] constexpr float Angle_x() const noexcept { return pitch; }
    [[nodiscard]] constexpr float Distance() const noexcept { return distance; }
    [[nodiscard]] constexpr glm::vec3 Center() const noexcept { return center; }


private:
    float yaw;   // equivalent of angleY in world space
    float pitch;   // equivalent of angleX in world space
    float distance, d_min, d_max;

    float zoom{ 1 };
    vec3 direction{ 0,0, 1 };
    vec3 center{ 0, 0, 0 };
    vec3 up{ 0, 1, 0 };
    vec3 right{ 1, 0, 0 };
    vec3 worldUp{ 0, 1, 0 };

    constexpr void ClampDistance() noexcept
    {
        if (distance < d_min)
            distance = d_min;
        else if (distance > d_max)
            distance = d_max;
    }

	void UpdateCameraVectors() noexcept;

};

class FpsCamera : public OrbitCamera
{
public:
    explicit FpsCamera(vec3 target) noexcept
        : OrbitCamera{ target, 0.0f, 0.0f }
    { }

    explicit FpsCamera(vec3 target, float min_distance, float max_distance = std::numeric_limits<float>::max(),
        float distance = 0.0f) noexcept
        : OrbitCamera{ target, min_distance, max_distance, distance }
    { }

    void MoveTarget(Camera_Movement action, float deltaTime) noexcept;
    using OrbitCamera::MoveTarget;
    using OrbitCamera::ObjectRotation;
    using OrbitCamera::Angle_y;
    using OrbitCamera::Angle_x;
    using OrbitCamera::Center;
    using OrbitCamera::ViewMatrix;

    /*Returns target's forward direction parallel to ground (XZ) plane*/
    [[nodiscard]] vec3 Forward() const noexcept;
    /*Returns target's right direction parallel to ground (XZ) plane*/
    [[nodiscard]] vec3 Right() const noexcept;
};

#endif

