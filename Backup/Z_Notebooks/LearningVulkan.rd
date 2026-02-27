<<Vulkan>>
	--#简介#:

        Vulkan默认没有farme buffer,而是需要显示的创建一个"Swap chain"的infrastructure,
	
	--#目录#:

	--#框架#:


	--#初始化/创建#:
        -实例化:
            VkApplicationInfo vk_info{};
                结构体:包含vulkan描述应用程序的基本信息
                    typedef struct VkApplicationInfo {
                    VkStructureType    sType;					枚举类型,当前这个结构体的类型:VkApplicationInfo结构体对应VK_STRUCTURE_TYPE_APPLICATION_INFO;
                                                                与pNext扩展结构体有密切联系,如果有扩展的结构体,它可以用其他的type.
                    const void*        pNext;					如果需要扩展该结构体,则指向扩展结构体,如果不需要则传null
                    const char*        pApplicationName;		应用程序的名称,主要用于调试工具的识别名称
                    uint32_t           applicationVersion;		应用程序的版本号,如VK_MAKE_VERSION(1, 0, 0)
                                                                应用程序自身的版本号（例如 v1.0.2）,主要用于调试识别,与vulkan本身无关
                    const char*        pEngineName;				基于一些引擎,比如unity或者ue的适配.原生开发-null 
                    uint32_t           engineVersion;			对应引擎版本,不需要则填0
                    uint32_t           apiVersion;				核心成员，指定应用目标的 Vulkan API 版本,驱动会确保提供与该版本兼容的功能，若应用调用了高于此版本的 API，可能会触发验证层错误
                                                                VK_API_VERSION_1_0
                } VkApplicationInfo;

            VkInstanceCreateInfo vk_createInfo{};
                结构体类型:包含之后要对其实例化的信息:
                    typedef struct VkInstanceCreateInfo {
                        VkStructureType             sType;						标识当前结构体的类型,VkInstanceCreateInfo对应VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO
                        const void*                 pNext;						扩展结构体,如果有可以填入扩展结构体的指针,没有则为空;
                        VkInstanceCreateFlags       flags;						用于未来版本的预留字段.
                        const VkApplicationInfo*    pApplicationInfo;			需要即将初始化好的VkApplicationInfo,传入
                        uint32_t                    enabledLayerCount;			用于验证层;
                        const char* const*          ppEnabledLayerNames;		指向字符串数组的指针，数组中是要启用的验证层名称
                        uint32_t                    enabledExtensionCount;		指定创建实例时需要启用的 Vulkan 扩展的数量（Vulkan 核心功能之外的可选功能，如窗口系统交互、调试等）
                        const char* const*          ppEnabledExtensionNames;	指向字符串数组的指针，数组中是要启用的扩展名称
                    } VkInstanceCreateInfo;
            
            Vulkan的实例化:
                VkResult vkCreateInstance(
                    const VkInstanceCreateInfo* pCreateInfo,  	// 参数1：创建实例的配置信息
                    const VkAllocationCallbacks* pAllocator,  	// 参数2：自定义内存分配器（可选）
                    VkInstance* pInstance                    	// 参数3：输出参数，接收创建的实例句柄
                );
        -扩展层:

        -验证层:

        -物理设备层:
            VkResult vkEnumeratePhysicalDevices(
                VkInstance                                  instance,
                uint32_t*                                   pPhysicalDeviceCount,
                VkPhysicalDevice*                           pPhysicalDevices);


        -队列:
            VkDeviceQueueCreateInfo:
                结构体,创建队列的一些信息;
                    typedef struct VkDeviceQueueCreateInfo {
                        VkStructureType             sType;
                        const void*                 pNext;
                        VkDeviceQueueCreateFlags    flags;
                        uint32_t                    queueFamilyIndex; 
                        uint32_t                    queueCount;       
                        const float*                pQueuePriorities; 
                    } VkDeviceQueueCreateInfo;

                queueFamilyIndex是指定的队列簇中的某一个Queue,像是银行不同部门,比如VIP部门,前台业务部门,个人业务部门等等.
                queueCount是队列簇中某一个Queue中类似窗口一样的东西,像是银行中有VIP1室,VIP2室,queueCount为1,则使用1个VIP室处理业务1,如果为2,则有两个VIP室处理业务,一个处理业务1一个处理业务2.



        队列和队列族:
            首先,是先有队列族再有队列.
            队列族:
                在Vulkan初始化队列的时候,会根据当前设备再初始化队列族,队列族是Vulkan对于GPU硬件的抽象.
                在GPU中,被划分了很多功能单元,对于数据处理有专门的分工,比如,图形渲染单元,通用计算单元,DMA传输单元和呈现单元等...
                队列族,不是GPU天生就有,而是Vulkan对GPU单元的重新映射,不是改变物理硬件的能力.
                此外队列族是具有独立功能的,比如有图形队列族,计算队列族,传输队列族,呈现队列族等等.

                代码体现:
                    首先需要通过vkEnumeratePhysicalDevices()获得有多少个设备,和具体的设备都有那些,并保存到一个容器中vector<VkPhysicalDevice_T *> devices(device_cout);
                    队列族是使用句柄获取一个特定的设备的句柄值,然后通过内核系统调用硬件的:
                        VkPhysicalDevice vk_physical_device;    硬件句柄
                        通过:
                            for (const auto &device : devices)
                            {
                                VkPhysicalDeviceProperties physical_device_properties;
                                vkGetPhysicalDeviceProperties(device, &physical_device_properties);
                                
                                VkPhysicalDeviceFeatures physical_device_feature;
                                vkGetPhysicalDeviceFeatures(device, &physical_device_feature);

                                if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && physical_device_feature.geometryShader)
                                {
                                    vk_physical_devices = device;
                                    break;
                                }
                            }
                            通过在设备容器中遍历,然后比对具有相同功能的设备并获取它的句柄值.

                        然后通过设备句柄获得有多少队列族:
                            vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, nullptr);
                        再通过一个容器保存这些具体的队列族:
                            std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
                            vkGetPhysicalDeviceQueueFamilyProperties(this->vk_physical_devices, &queue_family_count, queue_families.data());

                        番外:
                            在已经存储好所有队列族中的容器中查找,哪些可以进行渲染
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

                                    if (graphic_family_index > 0 && render_family_index > 0)
                                    {
                                        break;
                                    }
                                    i++;
                                }

                            graphic_family_index就是用来记录那队列族中的哪个可以进行VK_QUEUE_GRAPHICS_BIT功能的索引值.
                            vkGetPhysicalDeviceSurfaceSupportKHR(...)是用来检测当前设备中的当前队列族能传递到窗口中


            队列:
                队列是程序员,将使用一些队列族,使用队列族的功能.
                假如我创建 2 个队列,通过绑定,可以将这两个队列绑定图形队列族,这样这两个队列都可以执行图形队列族的功能.

        交换链:
            Vulkan不带有默认的缓冲区,则需要手动请求,通过Swap Chain才能将图像显示在屏幕上.
            队列的工作和队列呈现图像的方式都依靠Swap Chain.
            但最普遍的目的是Swap Chain使图像的显示和屏幕刷新率保持同步(synchronize).
                因为不是所有显卡都支持直接将图像显示在屏幕上,比如一些只为了计算的显卡.
                所以首先需要检查是否支持Swap Chain:
                    vkEnumerateDeviceExtensionProperties(...)用于通过物理设备句柄查询是否包含一些设备功能

            之后开始启用可用的扩展:
                










