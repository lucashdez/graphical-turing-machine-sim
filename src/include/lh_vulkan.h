#ifndef LH_VULKAN_H
#define LH_VULKAN_H

#ifdef _WIN32
#  define print(s) OutputDebugStringA(s)
#else
#  define print(s) printf(s)
#endif

typedef struct tag_VulkanImages{
	struct Arena arena;
	VkImage* images;
} VulkanImages;

typedef struct tag_VulkanImageViews {
	struct Arena arena;
	VkImageView* image_views;
} VulkanImageViews;

typedef struct tag_VulkanFramebuffer {
	struct Arena arena;
	VkFramebuffer* framebuffers;
} VulkanFramebuffers;

typedef struct tag_VulkanCommandBugger {
	struct Arena arena;
	VkCommandBuffer* command_buffers;
} VulkanCommandBuffers;

typedef struct tag_VulkanData {
  VkInstance instance;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice;
  VkDevice device;
  VkQueue graphicsQueue;
  VkSwapchainKHR swapchain;
  VulkanImages swapchainImages;
  VulkanImageViews swapchainImageViews;
  VkRenderPass renderPass;
  VulkanFramebuffers swapchainFramebuffers;
  VkCommandPool commandPool;
  VulkanCommandBuffers commandBuffers;
} VulkanData;

typedef struct tag_VulkanInitData {
	HWND hwnd;
	HINSTANCE hinstance;
} VulkanInitData;

static b32 lhvk_init_vulkan(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);
static b32 lhvk_create_instance(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);
static b32 lhvk_create_window_surface(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);
static b32 lhvk_create_physical_device(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);
static b32 lhvk_create_device(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);
static b32 lhvk_create_swapchain(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);
static b32 lhvk_create_image_views(VulkanData *vulkan_data);
static b32 lhvk_create_render_pass(VulkanData *vulkan_data);



#endif
