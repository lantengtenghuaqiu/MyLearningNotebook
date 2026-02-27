#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <cstdlib>
#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <fstream>

#include <cstring>

#include <set>

#include <iostream>

#define Printf(content, variables) printf(content, variables);

#define Error(boolean, msg)                \
    if (boolean)                           \
    {                                      \
        printf("%d:Wrong!!!\n", __LINE__); \
        throw std::runtime_error(msg);     \
        exit(0);                           \
    }

#define Write(file, msg) file << __LINE__ << " : " << msg << "\n";

#ifdef NDBUGE
const bool enable_or_disable_validation_layer = false;
#else
const bool enable_or_disable_validation_layer = false;
#endif
const std::vector<const char *> vk_validation_layer = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char*> device_extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

static std::ofstream file("logs.log");

void key_callback(GLFWwindow *window, int key, int action, int scand, int modes)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

class GLFW
{
public:
    GLFWwindow *glfw_window = nullptr;
    uint32_t window_width = 1280;
    uint32_t window_height = 720;
    const char *window_title = nullptr;

    GLFW() {}

    GLFW(uint32_t width, uint32_t height, const char *title) : window_width(width), window_height(height), window_title(title) {}

    void Init()
    {
        Error(glfwInit() == GLFW_FALSE, "GLFW Init Error!!!\n")

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->glfw_window = glfwCreateWindow(this->window_width, this->window_height, this->window_title, nullptr, nullptr);
    }

    void MainLoop()
    {
        glfwSetKeyCallback(this->glfw_window, key_callback);

        while (!glfwWindowShouldClose(this->glfw_window))
        {
            glfwPollEvents();
        }
    }

    void Destory()
    {
        glfwDestroyWindow(this->glfw_window);
        glfwTerminate();
    }
};

class Vulkan
{
public:
    VkInstance_T *vk_instance;
    VkPhysicalDevice_T *vk_physical_devices = VK_NULL_HANDLE;
    VkDevice_T *logic_device;
    VkQueue_T *graphics_queue;
    VkQueue_T *render_queue;
    VkSurfaceKHR_T *vk_surfaceHKR;

    VkResult vk_result;

    Vulkan() {}

    void Init(GLFWwindow *window)
    {
        VulkanExtensions();
        VulkanValidationLayer();
        CreateInstance();
        Win32Surface(window);
        GetPhysicalDevice();
        Queue();
        SweapChain();
    }

    void CreateInstance()
    {
        // 初始化vulkan的数据
        VkApplicationInfo vk_app_info = {};
        vk_app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vk_app_info.pNext = nullptr;
        vk_app_info.pApplicationName = "HelloVulkan";
        vk_app_info.apiVersion = VK_API_VERSION_1_0;
        vk_app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        vk_app_info.pEngineName = "No Engine";
        vk_app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        
        // 绑定创建vulkan实例需要的相关信息
        VkInstanceCreateInfo vk_create_info = {};
        vk_create_info.pNext = nullptr;
        vk_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vk_create_info.pApplicationInfo = &vk_app_info; //对于这里传入的是地址的原因是为了给之后真正进行创建逻辑的vkCreateInstance传递地址
                                                        //如果不是地址形式,则会复制大量数据,造成不必要的内存消耗
        {
            uint32_t glfw_externs_count_to_vulkan = 0;
            const char **glfw_extens_to_vulkan;
            glfw_extens_to_vulkan = glfwGetRequiredInstanceExtensions(&glfw_externs_count_to_vulkan);
            vk_create_info.enabledExtensionCount = glfw_externs_count_to_vulkan;
            vk_create_info.ppEnabledExtensionNames = glfw_extens_to_vulkan;
        }

        if (enable_or_disable_validation_layer)
        {
            vk_create_info.enabledLayerCount = static_cast<uint32_t>(vk_validation_layer.size());
            vk_create_info.ppEnabledLayerNames = vk_validation_layer.data();
        }
        else
        {
            vk_create_info.enabledLayerCount = 0;
        }
        // vk_create_info.flags = 0;

        // 创建vulkan实例
        this->vk_result = vkCreateInstance(&vk_create_info, nullptr, &vk_instance);

        Error((this->vk_result != VK_SUCCESS), "failed to create instance!\n")
    }

    void VulkanExtensions()
    {
        // 获取可用的扩展
        uint32_t externsions_cout = 0;

        vkEnumerateInstanceExtensionProperties(nullptr, &externsions_cout, nullptr);

        std::vector<VkExtensionProperties> vk_extension_properties(externsions_cout);

        vkEnumerateInstanceExtensionProperties(nullptr, &externsions_cout, vk_extension_properties.data());

        printf("available extensions:\n");

        for (const auto &vk_extension_property : vk_extension_properties)
        {
            Write(file, vk_extension_property.extensionName);
            printf("%s\n", vk_extension_property.extensionName);
        }
    }

    void VulkanValidationLayer()
    {
        uint32_t vk_layer_count = 0;
        vkEnumerateInstanceLayerProperties(&vk_layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(vk_layer_count);
        vkEnumerateInstanceLayerProperties(&vk_layer_count, available_layers.data());

        bool layer_found = false;
        {
            for (const char *layer_name : vk_validation_layer)
            {
                layer_found = false;
                for (const auto &properties : available_layers)
                {
                    Write(file, properties.layerName);
                    if (strcmp(layer_name, properties.layerName) == 0)
                    {
                        layer_found = true;
                        break;
                    }
                }
                if (layer_found == false)
                {
                    break;
                }
            }
            Error((enable_or_disable_validation_layer && !layer_found), "validation layers requested, but not available!\n")
        }
    }

    void GetPhysicalDevice()
    {

        uint32_t device_cout = 0;
        // 获取物理层设备的数量
        vkEnumeratePhysicalDevices(this->vk_instance, &device_cout, nullptr);
        Error((device_cout == 0), "failed to find GPUs with Vulkan support!\n");
        std::vector<VkPhysicalDevice_T *> devices(device_cout);
        // 获取物理层设备的具体种类返回给devices
        vkEnumeratePhysicalDevices(this->vk_instance, &device_cout, devices.data());

        for (const auto &device : devices)
        {
            VkPhysicalDeviceProperties physical_device_properties;
            vkGetPhysicalDeviceProperties(device, &physical_device_properties);
            
            VkPhysicalDeviceFeatures physical_device_feature;
            vkGetPhysicalDeviceFeatures(device, &physical_device_feature);

            Write(file, physical_device_properties.deviceName);
            // printf("%s\n",physical_device_properties.deviceType);
            if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physical_device_feature.geometryShader)
            {
                vk_physical_devices = device;
                break;
            }
        }
        Error(vk_physical_devices == VK_NULL_HANDLE, "failed to find a suitable GPU!\n")
    }

    void Win32Surface(GLFWwindow *window)
    {
        VkWin32SurfaceCreateInfoKHR win32Surface_create_info = {};

        win32Surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        win32Surface_create_info.hwnd = glfwGetWin32Window(window);
        win32Surface_create_info.hinstance = GetModuleHandle(nullptr);

        this->vk_result = vkCreateWin32SurfaceKHR(this->vk_instance, &win32Surface_create_info, nullptr, &this->vk_surfaceHKR);

        Error(this->vk_result != VK_SUCCESS, "failed to create window surface!");

        // glfwCreateWindowSurface(this->vk_instance , window , nullptr , &this->vk_surfaceHKR)
    }


    void Queue()
    {
        // Queue family
        uint8_t graphic_family_index = 0;
        uint8_t render_family_index = 0;

        {
            uint32_t queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, nullptr);
            printf("%d : QueueFamilyCount : %d\n", __LINE__, queue_family_count);
            Write(file , "队列族:");
            Write(file , queue_family_count);
            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, queue_families.data());

            int i = 0;
            VkBool32 render_support = false;

            for (const auto &queue_family : queue_families)
            {
                if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    graphic_family_index = i;
                }
                render_support = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(this->vk_physical_devices, i, this->vk_surfaceHKR, &render_support);
                if (render_support)
                {
                    render_family_index = i;
                }
                // printf("%d\n",render_family_index);
                if (graphic_family_index > 0 && render_family_index > 0)
                {
                    break;
                }

                i++;
            }
        }

        {
            // 指定队列和队列类型:
            std::vector<VkDeviceQueueCreateInfo> device_queue_create_infos;
            // std::set<uint32_t> queue_families = {graphic_family_index, render_family_index};
            float queue_prioity = 1;

            {
                //Graphic family index:
                VkDeviceQueueCreateInfo device_queue_graphic_create_info = {};
                device_queue_graphic_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                device_queue_graphic_create_info.queueFamilyIndex = graphic_family_index; // 要创建的队列簇的队列索引(队列簇中的哪一个队列)
                device_queue_graphic_create_info.queueCount = 1;
                device_queue_graphic_create_info.pQueuePriorities = &queue_prioity;
                device_queue_graphic_create_info.pNext = nullptr;
                device_queue_graphic_create_info.flags = 0;
                device_queue_create_infos.emplace_back(device_queue_graphic_create_info);

                //Render family index:
                VkDeviceQueueCreateInfo device_queue_render_create_info = {};
                device_queue_render_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                device_queue_render_create_info.queueFamilyIndex = render_family_index; // 要创建的队列簇的队列索引(队列簇中的哪一个队列)
                device_queue_render_create_info.queueCount = 1;
                device_queue_render_create_info.pQueuePriorities = &queue_prioity;
                device_queue_render_create_info.pNext = nullptr;
                device_queue_render_create_info.flags = 0;
                device_queue_create_infos.emplace_back(device_queue_render_create_info);
            }

            VkPhysicalDeviceFeatures physical_device_features = {};
            
            // 指定Logic device
            VkDeviceCreateInfo device_create_info = {};
            device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            device_create_info.queueCreateInfoCount = static_cast<uint32_t>(device_queue_create_infos.size());
            device_create_info.pQueueCreateInfos = device_queue_create_infos.data();
            device_create_info.pEnabledFeatures = &physical_device_features;
                //Swap Chain的启用

            device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
            device_create_info.ppEnabledExtensionNames = device_extensions.data();

            if (enable_or_disable_validation_layer)
            {
                device_create_info.enabledLayerCount = static_cast<uint32_t>(vk_validation_layer.size());
                device_create_info.ppEnabledLayerNames = vk_validation_layer.data();
            }
            else
            {
                device_create_info.enabledLayerCount = 0;
            }
            this->vk_result = 
            vkCreateDevice(this->vk_physical_devices, &device_create_info, nullptr, &this->logic_device);
            //vkCreateDeviceVulkan 中创建 “逻辑设备（VkDevice）” 的核心函数
            //物理设备是 “GPU 硬件本身”，逻辑设备是 “你程序操作 GPU 的‘遥控器’”
            //vkCreateDevice() 就是把 “硬件” 和 “遥控器” 绑定的关键一步，没有它，你无法让 GPU 做任何实际工作
        }
        Error(this->vk_result != VK_SUCCESS, "failed to create logical device!\n");

        vkGetDeviceQueue(this->logic_device, graphic_family_index, 0, &graphics_queue);
        vkGetDeviceQueue(this->logic_device, render_family_index, 0, &render_queue);
    }

    void SweapChain()
    {
        bool device_extensions_support = false;
        {
            uint32_t extensions_count = 0;
            
            vkEnumerateDeviceExtensionProperties(this->vk_physical_devices , nullptr , &extensions_count , nullptr);
            //是 Vulkan 中查询指定物理设备（GPU）支持的 “设备级扩展” 的核心函数
            //获取指定的物理设备(GPU)能支持的所有 “额外功能清单”（比如交换链、光线追踪、纹理压缩等）
            std::vector<VkExtensionProperties> avaliable_extensions(extensions_count);

            vkEnumerateDeviceExtensionProperties(this->vk_physical_devices , nullptr , &extensions_count , avaliable_extensions.data());
        
            std::set<std::string> require_extensions(device_extensions.begin(), device_extensions.end());
            // printf("%c",device_extensions.begin());

            Write(file , "Swap Chain:");
            for(const auto & exten : avaliable_extensions)
            {
                require_extensions.erase(exten.extensionName);
                Write(file , exten.extensionName);
            }
            device_extensions_support = require_extensions.empty();
        }


        VkSurfaceCapabilitiesKHR  capabilities;
        /*
            Vulkan中描述物理设备(GPU),对某一窗口的交换链能力是创建SwapChainKHR的关键
            因为不同显卡有不同能力,如:A 显卡支持最大 4K 图像，B 显卡只支持 1080P
            VkSurfaceCapabilitiesKHR是一个结构体,是渲染到窗口前进行设置渲染属性的设置的:
                typedef struct VkSurfaceCapabilitiesKHR {
                uint32_t        minImageCount;        // 1. 交换链最少需要的图像数量
                uint32_t        maxImageCount;        // 2. 交换链最多支持的图像数量
                VkExtent2D      currentExtent;        // 3. 窗口表面的“原生尺寸”（宽度x高度）
                VkExtent2D      minImageExtent;       // 4. 交换链图像的最小尺寸
                VkExtent2D      maxImageExtent;       // 5. 交换链图像的最大尺寸
                uint32_t        maxImageArrayLayers;  // 6. 图像数组层数（多视图渲染用，通常设1）
                VkSurfaceTransformFlagsKHR  supportedTransforms; // 7. 支持的图像变换模式
                VkSurfaceTransformFlagsKHR  currentTransform;    // 8. 窗口当前的变换模式
                VkCompositeAlphaFlagsKHR    supportedCompositeAlpha; // 9. 支持的Alpha合成模式
                VkImageUsageFlags           supportedUsageFlags;  // 10. 交换链图像支持的用途
            } VkSurfaceCapabilitiesKHR;
        */
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(this->vk_physical_devices , this->vk_surfaceHKR,&capabilities);
        //vulkan会根据vkGetPhysicalDeviceSurfaceCapabilitiesKHR中传入的逻辑设备,对VkSurfaceCapabilitiesKHR中的数据初始化
    
        
        /** */
        std::vector<VkSurfaceFormatKHR> formats;
        //Vulkan 中描述 “交换链图像的格式 + 颜色空间组合” 的核心结构体
        //必须从 GPU 提供的列表中选择，不能手动自定义
        uint32_t format_count = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(this->vk_physical_devices , this->vk_surfaceHKR , &format_count,nullptr);
        //是 Vulkan 中查询物理设备（GPU）对特定窗口表面（VkSurfaceKHR）支持的所有 “格式 + 颜色空间” 组合的核心函数 —— 它的唯一作用是返回 VkSurfaceFormatKHR
        /*
            VkResult vkGetPhysicalDeviceSurfaceFormatsKHR(
                VkPhysicalDevice        physicalDevice,        // 1. 目标GPU
                VkSurfaceKHR            surface,               // 2. 窗口表面
                uint32_t*               pSurfaceFormatCount,   // 3. 输出：支持的格式组合数量
                VkSurfaceFormatKHR*     pSurfaceFormats        // 4. 输出：格式组合列表（数组）
            );
        */
        if(format_count != 0)
        {
            formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(this->vk_physical_devices,this->vk_surfaceHKR , &format_count ,formats.data());
        }


        /** */
        std::vector<VkPresentModeKHR> present_modes;
        uint32_t present_modes_count;

        vkGetPhysicalDeviceSurfacePresentModesKHR(this->vk_physical_devices , this->vk_surfaceHKR , &present_modes_count , nullptr);
        if(present_modes_count!=0)
        {
            present_modes.resize(present_modes_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(this->vk_physical_devices , this->vk_surfaceHKR , &present_modes_count , present_modes.data());
        }

        bool swapChainAdequate = false;
        if(device_extensions_support)
        {
            swapChainAdequate != formats.empty() && !present_modes.empty();
        }

        //确定format的格式
        VkSurfaceFormatKHR format;
        {
            format = formats[0];

            for(const VkSurfaceFormatKHR &_format :formats)
            {
                if(_format.format == VK_FORMAT_B8G8R8A8_SRGB && _format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    format = _format;
                    break;
                }
            }
        }
        
        //确定format的格式



    }

    void Destory()
    {
        vkDestroySurfaceKHR(this->vk_instance, this->vk_surfaceHKR, nullptr);
        vkDestroyDevice(this->logic_device, nullptr);
        vkDestroyInstance(this->vk_instance, nullptr);
    }
};

class GLFW_VULKAN
{
public:
    GLFW glfw_window;
    Vulkan vulkan_application;

    GLFW_VULKAN()
    {
        glfw_window = GLFW(1280, 720, "Hello world");
        vulkan_application = Vulkan();
    }
    void Init()
    {
        // GLFW Init
        glfw_window.Init();

        // Vulkan Init
        vulkan_application.Init(glfw_window.glfw_window);
    }

    void MainLoop()
    {
        glfw_window.MainLoop();
    }

    void Destory()
    {
        vulkan_application.Destory();
        glfw_window.Destory();
    }
};

int main()
{
    GLFW_VULKAN Application = GLFW_VULKAN();
    try
    {
        Application.Init();
        Application.MainLoop();
        Application.Destory();
    }
    catch (const std::exception &e)
    {
        printf("%s \n", e.what());
    }

    return 0;
}