#pragma once
#ifndef H_MISCFUNC
#define H_MISCFUNC

#include "Window.h"

#include "Affine.h"
#include "imguiLayer.h"

#include "Camera.h"

#include "CubeMesh.h"
#include "QuadMesh.h"
#include "Model.h"
#include "Texture.h"
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
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;
bool firstMouse = true;

Model testModel;

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
    camera.ProcessMouseScroll(yoffset);
}
/* Continuously procss input */
inline void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, sys_time.deltaTime);
}

inline bool CheckCursor() 
{
    if (camera.Front.x >= 0 && camera.Front.x <= 0.2) 
    {
        if (camera.Front.y >= -0.4 && camera.Front.y <= -0.3) 
        {
            if(camera.Front.z >= 0.9 && camera.Front.z <= 0.92)
            {
                return true;
            }
        }
    }

    return false;
}

inline void Run(void)
{
    std::pair<int, int> sysWindowSize = sysWin->GetWindowSize();

    sys_time.time_last = glfwGetTime();
    imgui_layer = ImguiLayer(sysWindowSize.first, sysWindowSize.second, sysWin->GetWindow());

    imgui_layer.Init();

    imgui_layer.SetFunction(DisplayInfo);

    Shader normalShader("Shader/vertexShader.vert", "Shader/fragmentShader.frag");

    Texture ahegao;
    uint32_t aheTexture = ahegao.LoadTexture("Assets/Textures/Ahegaopng.png", true);

    //testModel.ImportModel("Assets/Models/cube.obj");
    testModel.ImportModel("Assets/Models/male/MaleLow.obj");

    normalShader.Use();

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

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        normalShader.SetMat4("projection", projection);
        normalShader.SetMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, aheTexture);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        normalShader.SetMat4("model", model);
        normalShader.SetBool("inverse_normals", false);
        testModel.Render();

        glBindVertexArray(0);
         
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
    //ImGui::SliderFloat3(": Cube Center", &cube_center.x, -2.0f, 2.0f);
    //ImGui::Button("Reset Center");
    //if (ImGui::Button("Reset Center"))
    //{
    //    cube_center = Point(0, 0, -1);
    //}
}

#endif // !H_MISCFUNC

