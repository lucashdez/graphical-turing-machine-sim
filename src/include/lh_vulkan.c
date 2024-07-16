static b32
lhvk_init_vulkan(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	lhvk_create_instance(vulkan_init_data, vulkan_data);
	lhvk_create_window_surface(vulkan_init_data, vulkan_data);
	lhvk_create_physical_device(vulkan_init_data, vulkan_data);
	lhvk_create_device(vulkan_init_data, vulkan_data);
	lhvk_create_swapchain(vulkan_init_data,vulkan_data);
	lhvk_create_image_views(vulkan_data);
	lhvk_create_render_pass(vulkan_data);

	return(true);
}

static b32
lhvk_create_instance(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	struct Arena scratch = mm_scratch_arena();
	VkApplicationInfo app_info = {0};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
	app_info.pApplicationName = "Vulkan graphical interface";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_3;
	u32 extension_count = 0;
	VkResult qresult = vkEnumerateInstanceExtensionProperties(0, &extension_count, 0);
	VkExtensionProperties* vk_extension_properties = MMPushArrayZeros(&scratch, VkExtensionProperties, extension_count);
	vkEnumerateInstanceExtensionProperties(0, &extension_count, vk_extension_properties);
	struct Arena extension_arena =  mm_scratch_arena();
	char** extensions = MMPushArrayZeros(&extension_arena, char*, extension_count);
	for (int i = 0; i < extension_count; ++i) {
		extensions[i] = MMPushArrayZeros(&extension_arena, char, VK_MAX_EXTENSION_NAME_SIZE);
		strcpy_s(extensions[i], VK_MAX_EXTENSION_NAME_SIZE, vk_extension_properties[i].extensionName);
	}
	u32 layer_count;
	struct Arena layer_arena = mm_scratch_arena();
	vkEnumerateInstanceLayerProperties(&layer_count, 0);
	VkLayerProperties* vk_layer_properties = MMPushArrayZeros(&layer_arena, VkLayerProperties, layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, vk_layer_properties);
	char** layers = MMPushArrayZeros(&layer_arena, char*, layer_count);
	for (int i = 0; i < layer_count; ++i) {
		layers[i] = MMPushArrayZeros(&layer_arena, char, VK_MAX_EXTENSION_NAME_SIZE);
		strcpy_s(layers[i], VK_MAX_EXTENSION_NAME_SIZE, vk_layer_properties[i].layerName);
	}
	VkInstanceCreateInfo create_info = {0};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;
	create_info.enabledExtensionCount = extension_count;
	create_info.ppEnabledExtensionNames = (const char* const*)extensions;
	create_info.enabledLayerCount = layer_count;
	create_info.ppEnabledLayerNames = (const char* const*)layers;

	print("Creating vulkan instance...\n");
	if (vkCreateInstance(&create_info, NULL, &vulkan_data->instance) != VK_SUCCESS) {
		print("FAILED TO CREATE INSTANCE");
		return(false);
	}
	print("Done\n");
	return(true);
}

static b32
lhvk_create_window_surface(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	print("Creating window surface\n");
#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR surface_create_info = {0};
	surface_create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; 
	surface_create_info.hwnd = vulkan_init_data.hwnd;
	surface_create_info.hinstance = vulkan_init_data.hinstance;
	surface_create_info.pNext = NULL;
	surface_create_info.flags = 0;
	VkResult result; 
	result = vkCreateWin32SurfaceKHR(vulkan_data->instance, &surface_create_info, NULL, &vulkan_data->surface);
	if(result != VK_SUCCESS) {
		switch (result) {
		case VK_ERROR_OUT_OF_HOST_MEMORY: {
			print("OUT OF HOST MEMORY");
		} break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY: {
			print("OUT OF DEVICE MEMORY");
		} break;
		default: break;
		}

		print("FAILED TO CREATE WINDOW SURFACE");
		return(false);
	}
#endif
	print("Done\n");
	return(true);
}


static b32 
lhvk_create_physical_device(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	print("Enumerating physical devices...\n");
	u32 device_count = 0;
	vkEnumeratePhysicalDevices(vulkan_data->instance, &device_count, NULL);
	if (device_count == 0) {
		print("FAILED TO DETECT DEVICES");
		return(false);
	}
	VkPhysicalDevice devices[2]; 
	vkEnumeratePhysicalDevices(vulkan_data->instance, &device_count, devices);
	vulkan_data->physicalDevice = devices[0]; 
	print("Done\n");
	return(true);
}

static b32
lhvk_create_device(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	struct Arena scratch = mm_scratch_arena();
	u32 extension_count = 0;
	vkEnumerateDeviceExtensionProperties(vulkan_data->physicalDevice, 0, &extension_count, 0);
	VkExtensionProperties *vk_extension_properties = MMPushArrayZeros(&scratch, VkExtensionProperties, extension_count);
	vkEnumerateDeviceExtensionProperties(vulkan_data->physicalDevice, 0, &extension_count, vk_extension_properties);

	char** extensions = MMPushArrayZeros(&scratch, char*, extension_count);
	for (int i = 0; i < extension_count; ++i) {
		extensions[i] = MMPushArrayZeros(&scratch, char, VK_MAX_EXTENSION_NAME_SIZE);
		strcpy_s(extensions[i], VK_MAX_EXTENSION_NAME_SIZE, vk_extension_properties[i].extensionName);
	}

	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = 0;
	queue_create_info.queueCount = 1;
	float queue_priority = 1.0f;
	queue_create_info.pQueuePriorities = &queue_priority;

	VkDeviceCreateInfo device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &queue_create_info;
	device_create_info.enabledExtensionCount = extension_count;
	device_create_info.ppEnabledExtensionNames = (const char* const*)extensions;

	typedef struct VkExtensionProperties {
		char        extensionName[VK_MAX_EXTENSION_NAME_SIZE];
		uint32_t    specVersion;
	} VkExtensionProperties;

	// FEATURES
	VkPhysicalDeviceFeatures physical_device_features;
	{
		vkGetPhysicalDeviceFeatures(vulkan_data->physicalDevice, &physical_device_features);
	}
	device_create_info.pEnabledFeatures = &physical_device_features;

	print("Creating device...\n");
	if(vkCreateDevice(vulkan_data->physicalDevice, &device_create_info, 0, &vulkan_data->device) != VK_SUCCESS) {
		print("CANNOT CREATE DEVICE\n");
		return(false);
	}
	print("Done\n");
	print("Getting device queue...\n");
	vkGetDeviceQueue(vulkan_data->device, 0, 0, &vulkan_data->graphicsQueue);
	print("Done\n");
	return(true);
}

static b32
lhvk_create_swapchain(VulkanInitData vulkan_init_data, VulkanData *vulkan_data) {
	print("Creating swapchain...\n");
	VkSwapchainCreateInfoKHR swapchain_create_info = {};
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.surface = vulkan_data->surface;
	swapchain_create_info.minImageCount = 2;
	swapchain_create_info.imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
	swapchain_create_info.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	// Surface 
	VkSurfaceCapabilitiesKHR surface_capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan_data->physicalDevice,
											  vulkan_data->surface,
											  &surface_capabilities);
	swapchain_create_info.imageExtent = surface_capabilities.currentExtent;
	//~
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	// ?
	swapchain_create_info.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(vulkan_data->device, &swapchain_create_info, 0, &vulkan_data->swapchain) != VK_SUCCESS) {
		print("Failed to create swapchain\n");
		return(false);
	}
	print("Done!\n");
	return(true);
}

static b32
lhvk_create_image_views(VulkanData *vulkan_data) {
	u32 imageCount = 0;
	vkGetSwapchainImagesKHR(vulkan_data->device, vulkan_data->swapchain, &imageCount, 0);
	vulkan_data->swapchainImages.arena = mm_scratch_arena();
	vulkan_data->swapchainImages.images = MMPushArrayZeros(&vulkan_data->swapchainImages.arena, VkImage, imageCount);
	vkGetSwapchainImagesKHR(vulkan_data->device, vulkan_data->swapchain, &imageCount, vulkan_data->swapchainImages.images);
	vulkan_data->swapchainImageViews.arena = mm_scratch_arena();
	vulkan_data->swapchainImageViews.image_views = MMPushArrayZeros(&vulkan_data->swapchainImageViews.arena, VkImageView, imageCount);
	for (int i = 0; i < imageCount; ++i) {
		VkImageViewCreateInfo create_info = {};
		create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info.image = vulkan_data->swapchainImages.images[i];
		create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info.format = VK_FORMAT_R8G8B8A8_SRGB;
		create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info.subresourceRange.baseMipLevel = 0;
		create_info.subresourceRange.levelCount = 1;
		create_info.subresourceRange.baseArrayLayer = 0;
		create_info.subresourceRange.layerCount = 1;

		if(vkCreateImageView(vulkan_data->device,
							 &create_info,
							 0,
							 &vulkan_data->swapchainImageViews.image_views[i]) != VK_SUCCESS) {
			print("CANNOT CREATE IMAGE VIEW");
		}
	}
	return(true);
}

static b32
lhvk_create_render_pass(VulkanData *vulkan_data) {
	VkAttachmentDescription color_attachment = {};
	color_attachment.format = VK_FORMAT_R8G8B8A8_SRGB;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 1;
    render_pass_info.pAttachments = &color_attachment;
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;

	if(vkCreateRenderPass(vulkan_data->device, &render_pass_info, 0, &vulkan_data->renderPass) != VK_SUCCESS) {
		print("CANNOT CREATE RENDER PASS");
		return(false);
	}
	return(true);
}
