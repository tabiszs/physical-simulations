#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "scene.h"

class Window {
private:
    GLFWwindow* m_Window;
    GLFWmonitor* m_Monitor;

    const char* m_Title;
    GLFWimage m_Icon[1];
 
    int m_PosX, m_PosY;
    bool m_Fullscreen;

    std::shared_ptr<Scene> scene;


    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    //modes
    int oldAction = GLFW_RELEASE;


    const int staticPixelSize = 1;
    int renderPixelSize = 4;
    int pixelSize = staticPixelSize;

public:
    int m_Width, m_Height;


public:
    Window(int width, int height, const char* title);
    ~Window()
    {
        glfwDestroyWindow(m_Window);
    }

    GLFWwindow* getWindow();
    void ImportScene(std::shared_ptr<Scene> scene);
private:
    void initGLFW();
    void createWindow();
    void introduceWindowToCurrentContext();

    void center();
    void setFullscreen();

    void setWindowSizeCallback();
    static void static_WindowSizeCallback(GLFWwindow* window, int width, int height);
    void windowSizeCallback(int width, int height);

    void setKeyCallback();
    static void static_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void keyCallback(int key, int scancode, int action, int mods);

    void setMouseCallback();
    static void static_MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void mouseCursorPosCallback(double xpos, double ypos);
    static void static_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseButtonCallback(int button, int action, int mods, double xpos, double ypos);


    void setScrollCallback();
    static void static_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void scrollCallback(double xoffset, double yoffset);

    float ScreenToClipSpaceX(float pos);
    float ScreenToClipSpaceY(float pos);
};
