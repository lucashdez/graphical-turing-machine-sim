internal VKAPI_ATTR VkBool32 VKAPI_CALL 
_lhvk_debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        printf( ColorError "[ERROR]: %s\n" ColorReset, pCallbackData->pMessage);
    } 
    else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) 
    {
        printf( ColorWarn "[WARNING]: %s\n" ColorReset, pCallbackData->pMessage);
    }
    else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        printf( ColorInfo "[INFO]: %s\n" ColorReset, pCallbackData->pMessage);
    }
    else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        printf( ColorDebug "[DEBUG]: %s\n" ColorReset, pCallbackData->pMessage);
    }
    
    return VK_FALSE;
}

internal b32
_lhvk_checkValidationLayerSupport() 
{
    const u32 validationLayersCount = 1;
    const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };
    uint32_t availableLayerCount;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, NULL);
    
    VkLayerProperties availableLayers[availableLayerCount];
    vkEnumerateInstanceLayerProperties(&availableLayerCount,availableLayers);
    
    for (int i = 0; i < validationLayersCount; ++i) {
        b32 layerFound = false;
        
        for (int j = 0; j < availableLayerCount; ++j) {
            if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        
        if (layerFound) {
            return true;
        }
    }
    
    return false;
}

internal void 
_lhvk_getRequiredExtensions(Lhvk_VulkanState* state, uint32_t* extensionCount, const char** pExtensions) {
    u32 glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    if (pExtensions == NULL){
        *extensionCount = glfwExtensionCount + (state->enable_validation_layers ? 1 : 0);
        return;
    }
    
    for (int i = 0; i < glfwExtensionCount; ++i){
        pExtensions[i] = glfwExtensions[i];
    }
    
    if (state->enable_validation_layers) {
        pExtensions[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }
}

internal void
_lhvk_populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo) 
{
    pCreateInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    pCreateInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    pCreateInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    pCreateInfo->pfnUserCallback = _lhvk_debugCallback;
    pCreateInfo->pNext = 0;
    pCreateInfo->flags = 0;
}



internal Lhvk_VulkanState 
lhvk_initVulkan(PlatformWindow* window)
{
    Lhvk_VulkanState state = {0}; 
    state.arena = mm_make_arena_reserve(GLOBAL_BASE_ALLOCATOR, KB(128));
    return (state);
}
internal void 
lhvk_createInstance(Lhvk_VulkanState* state)
{
    // TODO(lucashdez): solve this
    const u32 validationLayersCount = 1;
    const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };
    if (state->enable_validation_layers && !_lhvk_checkValidationLayerSupport())
    {
        printf("Validation requested but not available");
    }
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Vulkan",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = NULL,
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };
    
    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
    };
    
    uint32_t extensionCount = 0;
    _lhvk_getRequiredExtensions(state, &extensionCount, NULL);
    
    const char* extensions[extensionCount];
    _lhvk_getRequiredExtensions(state, &extensionCount, extensions);
    
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;
    createInfo.enabledLayerCount = 0;
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
    if (state->enable_validation_layers) {
        createInfo.enabledLayerCount = validationLayersCount;
        createInfo.ppEnabledLayerNames = validationLayers;
        
        _lhvk_populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = NULL;
    }
    
    if (vkCreateInstance(&createInfo, NULL, &state->instance) != VK_SUCCESS) {
        printf( "%s - unable to initialize Vulkan!\n", __FUNCTION__ );
    }
}

// 1225100077