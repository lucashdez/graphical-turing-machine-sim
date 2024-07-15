static b32
lhvk_init_vulkan(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	VkApplicationInfo app_info = {0};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
	app_info.pApplicationName = "Vulkan graphical interface";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info = {0};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	if (vkCreateInstance(&create_info, NULL, &vulkan_data->instance) != VK_SUCCESS) {
		printf("FAILED TO CREATE INSTANCE");
		return(false);
	}

#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR surface_create_info = {0};
	surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; 
	surface_create_info.hwnd = vulkan_init_data.hwnd;
	surface_create_info.hinstance = GetModuleHandle(NULL);
	if(vkCreateWin32SurfaceKHR(vulkan_data->instance, &surface_create_info, NULL, &vulkan_data->surface) != VK_SUCCESS) {
		printf("FAILED TO CREATE WINDOW SURFACE");
		return(false);
	}
#endif

	u32 device_count = 0;
	vkEnumeratePhysicalDevices(vulkan_data->instance, &device_count, NULL);
	if (device_count == 0) {
		printf("FAILED TO DETECT DEVICES");
		return(false);
	}

	VkPhysicalDevice devices[device_count]; 
	vkEnumeratePhysicalDevices(vulkan_data->instance, &device_count, devices);
	vulkan_data->physical_device = devices[0]; 

	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = 0;
	queue_create_info.queueCount = 1;
	float queue_priority = 1.0f;
	queue_create_info.pQueuePriorities = &queue_priority;
	
	return(true);
}
