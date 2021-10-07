#pragma once
#ifndef H_MISCFUNC
#define H_MISCFUNC

#include "Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Affine.h"
#include "imguiLayer.h"

#include "Camera.h"

#include "MeshRendering.h"
#include "CubeMesh.h"
#include "QuadMesh.h"
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

bool hdr = true;
bool hdrKeyPressed = false;
float exposure = 0.2f;

ImguiLayer imgui_layer;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = (float)SCREEN_WIDTH / 2.0;
float lastY = (float)SCREEN_HEIGHT / 2.0;
bool firstMouse = true;

CubeMesh cubeMesh;
QuadMesh quadMesh;
const int MAX_OBJECT = 20;

double beforeTimeSnapshot;
double afterTimeSnapshot;

void DisplayInfo();

float frand(float a = 0, float b = 1)
{
    return a + (b - a) * float(rand()) / float(RAND_MAX);
}

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void RenderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

inline uint32_t LoadTexture(const char* filepath, bool enableGamma)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = enableGamma ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = enableGamma ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << filepath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
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

    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, sys_time.deltaTime);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        exposure++;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        exposure--;*/
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
    Shader hdrShader("Shader/hdrVertex.vert", "Shader/hdrFragment.frag");
    Shader uiShader("Shader/vertexUI.vert", "Shader/fragmentUI.frag");

    // load textures
    // -------------
    uint32_t woodTexture = LoadTexture("Assets/Textures/wood.png", true); 
    uint32_t containerTexture = LoadTexture("Assets/Textures/container2.png", true);
    uint32_t metalTexture = LoadTexture("Assets/Textures/metal.png", true);

    //cubeMesh.GenVertexObject();
    //quadMesh.GenVertexObject();

    // configure floating point framebuffer
    // ------------------------------------
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // lighting info
    // -------------
    // positions
    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
    lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 1.0f));
    lightPositions.push_back(glm::vec3(0.0f, -1.8f, -0.2f));
    lightPositions.push_back(glm::vec3(0.8f, 0.0f, 0.4f));
    lightPositions.push_back(glm::vec3(0.0f, -0.5f, 0.9f));
    lightPositions.push_back(glm::vec3(0.1f, 1.5f, -1.0f));
    lightPositions.push_back(glm::vec3(0.0f, -1.7f, 4.2f));
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(100.0f, 100.0f, 100.0f));
    lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
    lightColors.push_back(glm::vec3(0.2f, 0.1f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.1f));
    lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.1f));

    // shader configuration
    // --------------------
    normalShader.Use();
    normalShader.SetInt("diffuseTexture", 0);
    hdrShader.Use();
    hdrShader.SetInt("hdrBuffer", 0);

    glm::mat4 objModel[MAX_OBJECT];
    for (size_t i = 0; i < MAX_OBJECT; i++)
    {
        objModel[i] = glm::mat4(1.0f);
        objModel[i] = glm::translate(objModel[i], glm::vec3(2.0f * cos(frand(0.0f, 360.0f)), 2.0f * cos(frand(0.0f, 360.0f)), 2.0f * cos(frand(0.0f, 360.0f))));
        objModel[i] = glm::scale(objModel[i], glm::vec3(frand(0.2f, 0.7f)));
        objModel[i] = glm::rotate(objModel[i], glm::radians(frand(0.0f, 90.0f)), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    }

    beforeTimeSnapshot = glfwGetTime();

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
        glClearColor(0.7f, 0.4f, 0.6f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw Begin */

        imgui_layer.GUI_Begin();
        imgui_layer.Render(sysWindowSize.first, sysWindowSize.second);
        imgui_layer.GUI_End();

        // render
        // ------

        // 1. render scene into floating point framebuffer
        // -----------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        normalShader.Use();
        normalShader.SetMat4("projection", projection);
        normalShader.SetMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        // set lighting uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            normalShader.SetVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            normalShader.SetVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }
        //normalShader.SetVec3("lights.Position", camera.Position);
        //normalShader.SetVec3("lights.Color", glm::vec3(100, 100, 100));
        normalShader.SetVec3("viewPos", camera.Position);
        // render tunnel
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 5.0f));
        normalShader.SetMat4("model", model);
        normalShader.SetInt("inverse_normals", true);
        cubeMesh.Render();

        // then create multiple cubes as the scenery
        glBindTexture(GL_TEXTURE_2D, containerTexture);

        for (size_t i = 0; i < MAX_OBJECT; i++) 
        {
            normalShader.SetMat4("model", objModel[i]);
            normalShader.SetInt("inverse_normals", false);
            cubeMesh.Render();
        }

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Render Sphere as target
        glBindTexture(GL_TEXTURE_2D, metalTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 4.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        normalShader.SetMat4("model", model);
        normalShader.SetInt("inverse_normals", false);
        RenderSphere();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
        // --------------------------------------------------------------------------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        hdrShader.SetInt("hdr", hdr);
        hdrShader.SetFloat("exposure", exposure);
        quadMesh.Render();

        //std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;

        /* Draw End */

        afterTimeSnapshot = glfwGetTime();

        if (afterTimeSnapshot - beforeTimeSnapshot > 6.0) 
        {
            L_SYSTEM_WARN("You haven't found it yet??");
            break;
        }

        if(CheckCursor())
        {
            L_SYSTEM_INFO("You found the object!!");
            L_SYSTEM_TRACE("You found the object within {0} seconds", afterTimeSnapshot - beforeTimeSnapshot);
            break;
        }

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

