#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <stdio.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <fstream>

#include <cstring>

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
    VkQueue_T * graphics_queue;
    VkResult vk_result;

    Vulkan() {}

    void Init()
    {
        VulkanExtensions();
        VulkanValidationLayer();
        CreateInstance();
        GetPhysicalDevice();
        Queue();
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
        vk_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vk_create_info.pApplicationInfo = &vk_app_info;
        vk_create_info.pNext = nullptr;

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
            Write(file, physical_device_properties.deviceName);

            VkPhysicalDeviceFeatures physical_device_feature;
            vkGetPhysicalDeviceFeatures(device, &physical_device_feature);

            if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physical_device_feature.geometryShader)
            {
                vk_physical_devices = device;
                break;
            }
        }
        Error(vk_physical_devices == VK_NULL_HANDLE, "failed to find a suitable GPU!\n")
    }

    void Queue()
    {
        // Queue family
        uint8_t graphic_family_index = 0;

        {
            uint32_t queue_family_count = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, nullptr);
            printf("%d : QueueFamilyCount : %d\n", __LINE__, queue_family_count);

            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
            vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, queue_families.data());

            int i = 0;
            for (const auto &queue_family : queue_families)
            {
                if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    graphic_family_index = i;
                }
                if (graphic_family_index > 0)
                {
                    break;
                }
                i++;
            }
        }

        // 指定队列和队列类型:
        VkDeviceQueueCreateInfo device_queue_create_info = {};
        device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_info.queueFamilyIndex = graphic_family_index; // 要创建的队列簇的队列索引(队列簇中的哪一个队列)
        device_queue_create_info.queueCount = 1;
        float queue_prioity = 1;
        device_queue_create_info.pQueuePriorities = &queue_prioity;
        device_queue_create_info.pNext = nullptr;
        device_queue_create_info.flags = 0;

        // 指定Logic device
        VkPhysicalDeviceFeatures physical_device_features = {};

        VkDeviceCreateInfo device_create_info = {};
        device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_create_info.pQueueCreateInfos = &device_queue_create_info;
        device_create_info.queueCreateInfoCount = 1;
        device_create_info.pEnabledFeatures = &physical_device_features;

        device_create_info.enabledExtensionCount = 0;

        if (enable_or_disable_validation_layer)
        {
            device_create_info.enabledLayerCount = static_cast<uint32_t>(vk_validation_layer.size());
            device_create_info.ppEnabledLayerNames = vk_validation_layer.data();
        }
        else
        {
            device_create_info.enabledLayerCount = 0;
        }
        this->vk_result = vkCreateDevice(this->vk_physical_devices, &device_create_info, nullptr, &this->logic_device);
        Error(this->vk_result != VK_SUCCESS, "failed to create logical device!\n");

        vkGetDeviceQueue(this->logic_device , graphic_family_index , 0 , &graphics_queue);

    }

    void Destory()
    {
        vkDestroyDevice(this->logic_device , nullptr);
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
        vulkan_application.Init();
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