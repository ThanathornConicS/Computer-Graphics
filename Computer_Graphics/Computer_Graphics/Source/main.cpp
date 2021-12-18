#include <pch.h>

#define STB_IMAGE_IMPLEMENTATION

#include "VulkanApp.h"

int main(int argc, char* argv[])
{
    HANDLE mutex = CreateMutex(NULL, TRUE, (LPCWSTR)(APP_NAME));
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

    VulkanApp app;

    try 
    {
        app.RunVulkanApp();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

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

    return EXIT_SUCCESS;
}