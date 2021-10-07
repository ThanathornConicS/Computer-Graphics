#include <pch.h>

#include "MiscFunc.h"

int main(int argc, char* argv[])
{
    HANDLE mutex = CreateMutex(NULL, TRUE, L"Computer Graphics");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, L"Multiple Instances of Application", NULL, MB_ICONERROR | MB_OK);
        return 0;
    }

    _CrtMemState sOld;
    _CrtMemState sNew;
    _CrtMemState sDiff;
    _CrtMemCheckpoint(&sOld); //take a snapchot

    Log::Log::Init();
    L_SYSTEM_INFO("Initializing Log");

    /* Window Initialization */

    sysWin = &Window::GetInstance();

    /* Window Initialization */
    //glfwSetKeyCallback(sysWin->GetWindow(), KeyPressedCallback);
    glfwSetCursorPosCallback(sysWin->GetWindow(), CursorPosCallback);
    glfwSetScrollCallback(sysWin->GetWindow(), MouseScrollCallback);

    // Prepare Shader
    //progID = LoadShaders("vertexShader.vs", "fragmentShader.fs");
    //glUseProgram(progID);

    Run();

    Sleep(2000);

    L_SYSTEM_INFO("Closing window...");
    L_SYSTEM_INFO("System Shutdown");
    imgui_layer.Terminate();
    glfwTerminate();

    _CrtMemCheckpoint(&sNew); //take a snapshot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
        _CrtMemDumpStatistics(&sDiff);
        OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
        _CrtMemDumpAllObjectsSince(&sOld);
        OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
        _CrtDumpMemoryLeaks();
    }

    return 0;
}