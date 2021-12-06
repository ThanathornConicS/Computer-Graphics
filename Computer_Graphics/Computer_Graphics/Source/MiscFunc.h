#pragma once
#ifndef H_MISCFUNC
#define H_MISCFUNC

//#include "pch.h"

#include "Window.h"

#include "Affine.h"
#include "imguiLayer.h"

#include "Texture.h"
#include "Camera.h"

#include "RayQuadMesh.h"
#include "Shader.h"

struct Time
{
    double time_last;
    double current_time;
    int frame_count;
    double frame_time;
    double deltaTime;

    Time(double tl = 0, double ct = 0, int fc = 0, double ft = 0, double dt = 0)
        :time_last(tl), current_time(ct), frame_count(fc), frame_time(ft), deltaTime(dt)
    {}
};

GLuint progID;

Window* sysWin = nullptr;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
Time sys_time;

ImguiLayer imgui_layer;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, -7.0f));
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;
bool firstMouse = true;
float rotRate = 1.0f;

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void DisplayInfo();

float frand(float a = 0, float b = 1)
{
    return a + (b - a) * float(rand()) / float(RAND_MAX);
}

/* Process input only when there are activities */
inline void KeyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == 0)
        return;

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        exit(0);
        break;
    }
}
/* Process mouse input when there are activities */
inline void CursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    //L_TRACE("{0}, {1}", xPos, yPos);
    //L_TRACE("Camera-Look-At: ({0}, {1}, {2})", camera.Front.x, camera.Front.y, camera.Front.z);

    camera.ProcessMouseMovement(xoffset, yoffset);
}
/* Process mouse scroll when there are activities */
inline void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(-yoffset);
}
/* Continuously procss input */
inline void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        rotRate += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rotRate -= 0.01f;

    L_TRACE("Rot Rate: {0}", rotRate);
}

inline void Run(void)
{
    std::pair<int, int> sysWindowSize = sysWin->GetWindowSize();

    sys_time.time_last = glfwGetTime();

    imgui_layer = ImguiLayer(sysWindowSize.first, sysWindowSize.second, sysWin->GetWindow());
    imgui_layer.Init();
    imgui_layer.SetFunction(DisplayInfo);

    Shader skyBoxShader("Shader/rayMarch.vert", "Shader/rayMarch.frag");
    Shader rayMarchShader("Shader/rayMarch.vert", "Shader/rayMarch.frag");

    // skybox VAO
    /*unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);*/

    std::vector<std::string> faces
    {
        "Assets/CubeMaps/posx.jpg",
        "Assets/CubeMaps/negx.jpg",
        "Assets/CubeMaps/posy.jpg",
        "Assets/CubeMaps/negy.jpg",
        "Assets/CubeMaps/posz.jpg",
        "Assets/CubeMaps/negz.jpg"
    };
    Texture cubeMap(faces);

    rayMarchShader.Use();
    RayQuadMesh rayquad;
    rayquad.GenVertexObject();

    do
    {
        double t = glfwGetTime();
        sys_time.deltaTime = t - sys_time.time_last;
        sys_time.time_last = t;

        // frame rate
        ++sys_time.frame_count;
        sys_time.frame_time += sys_time.deltaTime;

        if (sys_time.frame_time >= 0.5)
        {
            double fps = sys_time.frame_count / sys_time.frame_time;
            sys_time.frame_count = 0;
            sys_time.frame_time = 0;
            std::stringstream ss;
            ss << sysWin->GetWindowName() << " [fps=" << int(fps) << "]";
            glfwSetWindowTitle(sysWin->GetWindow(), ss.str().c_str());
        }

        ProcessInput(sysWin->GetWindow());

        // clear the screen
        glClearColor(0.4f, 0.1f, 0.3f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw Begin */

        imgui_layer.GUI_Begin();
        imgui_layer.Render(sysWindowSize.first, sysWindowSize.second);
        imgui_layer.GUI_End();

        rayMarchShader.Use();

        rayMarchShader.SetVec3("Eye", camera.Position);

        rayMarchShader.SetFloat("SystemTime", sys_time.current_time);
        rayMarchShader.SetVec2("SystemResolution", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        rayMarchShader.SetFloat("rotateRate", rotRate);
        rayMarchShader.SetFloat("zoom", camera.Zoom);
        rayquad.Render();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.GetTextureID());

        /* Draw End */

        // swap in the back buffer
        glfwSwapBuffers(sysWin->GetWindow());
        glfwPollEvents();
        sys_time.current_time += sys_time.deltaTime;

    } while (glfwGetKey(sysWin->GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(sysWin->GetWindow()) == 0);

}

/* Attach with ImGui Element */
inline void DisplayInfo()
{
    ImGui::Text("Time: %.3f sec", glfwGetTime());
    ImGui::Text("FPS: %.3f", ImGui::GetIO().Framerate);

    //ImGui::Checkbox(": Draw Solid", &draw_solid);
    //ImGui::SliderFloat3(": Zoom", &zoom, 1.0f, 5.0f);
    /*ImGui::Button("Reset Center");
    if (ImGui::Button("Reset Center"))
    {
        cube_center = Point(0, 0, -1);
    }*/
}

#endif // !H_MISCFUNC

