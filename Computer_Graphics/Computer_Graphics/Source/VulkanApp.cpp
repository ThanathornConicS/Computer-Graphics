#include "pch.h"
#include "VulkanApp.h"

void VulkanApp::InitWindow()
{
    // GLFW
    if (!glfwInit())
    {
        L_SYSTEM_ERROR("Error: Cannot Initialize GLFW");
        return;
    }

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APP_NAME, nullptr, nullptr);
}
void VulkanApp::InitVulkan()
{
    CreateVulkanInstance();
    SetupDebugMessenger();
    PickPhysicalDevice();
}
void VulkanApp::Run()
{
    do
    {

        glfwPollEvents();

    } while (glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == 0);
}
void VulkanApp::Clean()
{
    if (enableValidationLayers)
        DestroyDebugUtilsMessengerEXT(v_instance, debugMessenger, nullptr);

    vkDestroyInstance(v_instance, nullptr);

    L_SYSTEM_INFO("Closing window...");
    glfwDestroyWindow(m_window);

    L_SYSTEM_INFO("System Shutting down...");
    glfwTerminate();
}

void VulkanApp::CreateVulkanInstance()
{
    if (enableValidationLayers && !CheckValidationLayerSupport())
        throw std::runtime_error("Error: Validation Layers Requested, But Not Available");

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = APP_NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = GetRequiredExtension();

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        PopulateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }


    if (vkCreateInstance(&createInfo, nullptr, &v_instance) != VK_SUCCESS)
        throw std::runtime_error("Error: Failed To Create Vulkan Instance");
}

void VulkanApp::PickPhysicalDevice()
{
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(v_instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("Error: Failed To Find GPU(s) With Vulkan Support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(v_instance, &deviceCount, devices.data());

    for (const auto& device : devices) 
    {
        if (IsDeviceSuitable(device)) 
        {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE)
        throw std::runtime_error("Error: Failed To Find Suitable GPU(s)");
}

std::vector<const char*> VulkanApp::GetRequiredExtension()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

bool VulkanApp::CheckValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayer(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayer.data());

    for(const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProps : availableLayer) 
        {
            if (strcmp(layerName, layerProps.layerName) == 0) 
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
            return false;
    }

    return true;
}
void VulkanApp::SetupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    PopulateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(v_instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        throw std::runtime_error("Error: Failed To Set Up Debug Meesenger");
}
void VulkanApp::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DebugCallback;
}

bool VulkanApp::IsDeviceSuitable(VkPhysicalDevice device)
{
    return true;
}
