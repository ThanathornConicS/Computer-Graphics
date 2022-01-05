#include "pch.h"
#include "Window.h"

Window* Window::s_instance = nullptr;

Window::Window()
{
    SetWindowSize(width, height);
    SetWindowPos(xpos, ypos);
    SetWindowName(name);

    // Initialize GLFW
    if (glfwInit() != GLFW_TRUE)
    {
        L_SYSTEM_ERROR("Error: Cannot Initialize GLFW");
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_windowName, NULL, NULL);

    if (!m_window)
    {
        L_SYSTEM_ERROR("Error: Failed to open GLFW window, check available version with GLView");
        m_initStatus = false;
    }

    glfwMakeContextCurrent(m_window);

    // GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        L_SYSTEM_ERROR("Error: {0}", glewGetErrorString(err));
        m_initStatus = false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowPos(m_window, m_xpos, m_ypos);
}
Window::~Window()
{}

void Window::SetWindowSize(int w, int h)
{
    m_width = w;
    m_height = h;
}
void Window::SetWindowPos(int x, int y)
{
    m_xpos = x;
    m_ypos = y;
}
void Window::SetWindowName(const char* n)
{
    m_windowName = n;
}

std::pair<int, int> Window::GetWindowSize() const
{
    return std::pair<int, int>(m_width, m_height);
}
std::pair<int, int> Window::GetWindowPos() const
{
    return std::pair<int, int>(m_xpos, m_ypos);
}
const char* Window::GetWindowName() const
{
    return m_windowName;
}

GLFWwindow* Window::GetWindow() const
{
    return m_window;
}
