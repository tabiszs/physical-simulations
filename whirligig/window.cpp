#include "window.h"
#include <algorithm>
#include <stdexcept>
#include <string>

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

Window::Window(int width, int height, const char* title)
    : m_Fullscreen(false), m_Width(width), m_Height(height), m_Title(title) {
    initGLFW();
    m_Monitor = glfwGetPrimaryMonitor();
    createWindow();
    introduceWindowToCurrentContext();
    setWindowSizeCallback();
    setKeyCallback();
    setMouseCallback();
    setScrollCallback();
    gladLoadGL();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

GLFWwindow* Window::getWindow() {
    return m_Window;
}

void Window::ImportScene(std::shared_ptr<Scene> s)
{
    scene = s;
}

void Window::initGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW!");
    }
}

void Window::createWindow() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
    if (!m_Window) {
        throw std::runtime_error("Could not create GLFW window!");
    }
    glfwSetWindowUserPointer(m_Window, this);
    center();
}

void Window::introduceWindowToCurrentContext()
{
    glfwMakeContextCurrent(m_Window);
}

void Window::center() {
    const GLFWvidmode* vidMode = glfwGetVideoMode(m_Monitor);
    glfwSetWindowPos(m_Window, (vidMode->width - m_Width) / 2, (vidMode->height - m_Height) / 2);
}

void Window::setFullscreen() {
    if (!m_Fullscreen) {
        const GLFWvidmode* vidMode = glfwGetVideoMode(m_Monitor);
        glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);
        glfwGetWindowSize(m_Window, &m_Width, &m_Height);
        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, vidMode->width, vidMode->height, vidMode->refreshRate);
        glfwSetWindowSize(m_Window, vidMode->width, vidMode->height);
        m_Fullscreen = !m_Fullscreen;
    }
    else {
        glfwSetWindowMonitor(m_Window, nullptr, m_PosX, m_PosY, m_Width, m_Height, 0);
        glfwSetWindowSize(m_Window, m_Width, m_Height);
        m_Fullscreen = !m_Fullscreen;
    }
}

void Window::setWindowSizeCallback() {
    glfwSetWindowSizeCallback(m_Window, static_WindowSizeCallback);
}

void Window::static_WindowSizeCallback(GLFWwindow* window, int width, int height) {
    Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
    actualWindow->windowSizeCallback(width, height);
}

void Window::windowSizeCallback(int width, int height) {
    glfwSetWindowSize(m_Window, width, height);
    scene->SetViewport(width, height);
    m_Height = height;
    m_Width = width;
    scene->UpdateViewFrustum(m_Width, m_Height);
}

void Window::setKeyCallback() {
    glfwSetKeyCallback(m_Window, static_KeyCallback);
}

void Window::static_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
    actualWindow->keyCallback(key, scancode, action, mods);
}

void Window::keyCallback(int key, int scancode, int action, int mods) {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard && scene->camera_movement)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(m_Window, true);
        if (key == GLFW_KEY_F11 && action == GLFW_RELEASE)
            setFullscreen();

        auto& camera = scene->camera;
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = std::min(currentFrame - lastFrame, 0.1f);
        lastFrame = currentFrame;
        if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
            camera->MoveTarget(FORWARD, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
            camera->MoveTarget(BACKWARD, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
            camera->MoveTarget(LEFT, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
            camera->MoveTarget(RIGHT, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_E) == GLFW_PRESS)
            camera->MoveTarget(UP, deltaTime);
        if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
            camera->MoveTarget(DOWN, deltaTime);
    }
}

void Window::setMouseCallback()
{
    glfwSetCursorPosCallback(m_Window, static_MouseCursorPosCallback);
    glfwSetMouseButtonCallback(m_Window, static_MouseButtonCallback);
}

void Window::static_MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
    actualWindow->mouseCursorPosCallback(xpos, ypos);
}

void Window::mouseCursorPosCallback(double xpos, double ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && scene->camera_movement)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            auto& camera = scene->camera;
            camera->Rotate(xoffset * ROTATION_SPEED, yoffset * ROTATION_SPEED);
        }
    }
    lastX = xpos;
    lastY = ypos;
}

void Window::static_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    actualWindow->mouseButtonCallback(button, action, mods, xpos, ypos);
}

void Window::mouseButtonCallback(int button, int action, int mods, double xpos, double ypos)
{

}

void Window::setScrollCallback()
{
    glfwSetScrollCallback(m_Window, static_ScrollCallback);
}

void Window::static_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* actualWindow = (Window*)glfwGetWindowUserPointer(window);
    actualWindow->scrollCallback(xoffset, yoffset);
}

void Window::scrollCallback(double xoffset, double yoffset)
{
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureMouse && scene->camera_movement)
    {
        auto& camera = scene->camera;
        camera->Zoom(yoffset * ZOOM_SPEED);
    }
}

float Window::ScreenToClipSpaceX(float xpos)
{
    return (xpos / static_cast<float>(m_Width)) * 2 - 1;
}

float Window::ScreenToClipSpaceY(float ypos)
{
    auto yClipPos =  (ypos / static_cast<float>(m_Height)) * 2 - 1;
    return yClipPos * -1;
}