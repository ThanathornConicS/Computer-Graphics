#pragma once
#ifndef H_MISCFUNC
#define H_MISCFUNC

#include "Window.h"

#include "imguiLayer.h"

#include "Texture.h"
#include "Camera.h"

#include "RayQuadMesh.h"
#include "SkyboxMesh.h"
#include "QuadMesh.h"
#include "Shader.h"
#include "HDRBuffer.h"

#include "Time.h"

#include "gtc/matrix_transform.hpp"

GLuint progID;

Window* sysWin = nullptr;
Time sysTime;

//ImguiLayer imgui_layer;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, -7.0f));
float lastX = (float)SCREEN_WIDTH / 2.0f;
float lastY = (float)SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float rotRate = 1.0f;
float zoom = 1.0f;

float exposure = 1.0f;

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
    camera.ProcessMouseScroll(yoffset, zoom);
}
/* Continuously procss input */
inline void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        rotRate += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        rotRate -= 0.1f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        exposure += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        exposure -= 0.01f;

    /*L_TRACE("Rot Rate: {0}", rotRate);
    L_TRACE("Exposure: {0}", exposure);*/
}

inline void Run(void)
{
    sysTime.time_last = glfwGetTime();

    Shader skyBoxShader("Shader/skyBox.vert", "Shader/skyBox.frag");
    Shader rayMarchShader("Shader/rayMarch.vert", "Shader/rayMarch.frag");
    Shader hdrShader("Shader/hdrVertex.vert", "Shader/hdrFragment.frag");

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

    HDRBuffer hdrBuffer;
    hdrBuffer.CreateBuffer();
    QuadMesh quadMesh;
    quadMesh.GenVertexObject();

    skyBoxShader.Use();
    skyBoxShader.SetInt("skybox", 0);
    SkyboxMesh skybox;
    skybox.GenVertexObject();

    rayMarchShader.Use();
    RayQuadMesh rayquad;
    rayquad.GenVertexObject();

    glm::mat4 shaderRotMat = glm::mat4(1.0f);

    do
    {
        sysTime.ProcessTime(sysWin->GetWindow(), glfwGetTime());

        ProcessInput(sysWin->GetWindow());

        shaderRotMat = glm::rotate(shaderRotMat, (float)(rotRate * sysTime.deltaTime), glm::vec3(0, 1, 1));

        // clear the screen
        glClearColor(0.4f, 0.1f, 0.3f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw Begin */

        hdrBuffer.BindBuffer();

        rayMarchShader.Use();

        rayMarchShader.SetFloat("SystemTime", sysTime.current_time);
        rayMarchShader.SetVec2("SystemResolution", glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        rayMarchShader.SetFloat("zoom", zoom);
        rayMarchShader.SetMat4("rotMat", shaderRotMat);

        rayquad.Render();

        // draw skybox as last
        skyBoxShader.Use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyBoxShader.SetMat4("view", view);
        skyBoxShader.SetMat4("projection", projection);

        skybox.Render(&cubeMap);

        // Render Everything as hdr buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrBuffer.m_colorBuffer);
        hdrShader.SetFloat("exposure", exposure);
        quadMesh.Render();

        /* Draw End */

        // Swap in the back buffer
        glfwSwapBuffers(sysWin->GetWindow());
        glfwPollEvents();
        sysTime.current_time += sysTime.deltaTime;

    } while (glfwGetKey(sysWin->GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(sysWin->GetWindow()) == 0);

}

/* Attach with ImGui Element */
//inline void DisplayInfo()
//{
//    ImGui::Text("Time: %.3f sec", glfwGetTime());
//    ImGui::Text("FPS: %.3f", ImGui::GetIO().Framerate);
//
//    ImGui::Checkbox(": Draw Solid", &draw_solid);
//    ImGui::SliderFloat3(": Zoom", &zoom, 1.0f, 5.0f);
//    /*ImGui::Button("Reset Center");
//    if (ImGui::Button("Reset Center"))
//    {
//        cube_center = Point(0, 0, -1);
//    }*/
//}

#endif // !H_MISCFUNC

