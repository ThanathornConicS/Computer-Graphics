#include <pch.h>
#define STB_IMAGE_IMPLEMENTATION

#include "MiscFunc.h"

int main(int argc, char* argv[])
{
    HANDLE mutex = CreateMutex(NULL, TRUE, (LPCWSTR)(APPNAME));
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

    sysWin = &Window::GetInstance();

    glfwSetScrollCallback(sysWin->GetWindow(), MouseScrollCallback);

    Run();

    L_SYSTEM_INFO("Closing window...");
    glfwDestroyWindow(sysWin->GetWindow());
    L_SYSTEM_INFO("System is Shutting down...");
    glfwTerminate();

    _CrtMemCheckpoint(&sNew); //take a snapshot 
    if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
    {
        OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
        _CrtMemDumpStatistics(&sDiff);
        /*OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
        _CrtMemDumpAllObjectsSince(&sOld);*/
        OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
        _CrtDumpMemoryLeaks();
    }

    return 0;
}