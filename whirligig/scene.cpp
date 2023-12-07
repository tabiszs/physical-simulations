#include "scene.h"
#include "mat.h"

void Scene::SetDevice(shared_ptr<Device> device)
{
	this->device = device;
}

void Scene::SetViewport(float width, float height)
{
	glViewport(0, 0, width, height);
}

void Scene::UpdateViewFrustum(int width, int height)
{
	viewFrustrum->setViewportSize({ width, height });
}

void Scene::DrawOn(std::shared_ptr<Device> device)
{
}

void Scene::Update()
{
}

void Scene::Menu()
{
}

void Scene::ProcessMouseCursorPosCallback(GLFWwindow* m_Window, float xpos, float ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && camera_movement)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            camera->Rotate(xoffset * ROTATION_SPEED, yoffset * ROTATION_SPEED);
        }
    }
    lastX = xpos;
    lastY = ypos;
}

void Scene::UpdateProjViewMtx()
{
	m_viewProjMtx = viewFrustrum->getProjectionMatrix() * camera->ViewMatrix();	
}

void Scene::SetProjViewMtx(const std::shared_ptr<Shader> shader)
{
	shader->use();
	shader->setMatrix4F("projViewMtx", m_viewProjMtx);
}

glm::vec4 Scene::ClipToWorldSpace(glm::vec4 clipPos)
{
    glm::mat4 invProjMtx = glm::inverse(viewFrustrum->getProjectionMatrix());
    glm::mat4 invViewMtx = glm::inverse(camera->ViewMatrix());
    auto newWorldPos = invViewMtx * invProjMtx * clipPos;
    newWorldPos /= newWorldPos.w;
    return newWorldPos;
}

glm::vec4 Scene::WorldToClipSpace(glm::vec4 worldPos)
{
    auto clippedCoords = viewFrustrum->getProjectionMatrix() * camera->ViewMatrix() * worldPos;
    clippedCoords /= clippedCoords.w;
    return clippedCoords;
}

glm::vec4 Scene::ScreenToClipSpace(float x_sceen_pos, float y_screen_pos, float z_clip_pos)
{
    const auto& viewport = viewFrustrum->viewportSize();
    float xClipPos = (x_sceen_pos / static_cast<float>(viewport.cx)) * 2 - 1;
    float yClipPos = (y_screen_pos / static_cast<float>(viewport.cy)) * 2 - 1;
    yClipPos *= -1;

    return glm::vec4(xClipPos, yClipPos, z_clip_pos, 1.0f);
}
