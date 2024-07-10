#ifndef LH_VULKAN_H
#define LH_VULKAN_H

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
  VkPhysicalDevice physical_device;
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
	void* hwnd;
} VulkanInitData;

static b32 lhvk_init_vulkan(VulkanInitData vulkan_init_data, VulkanData *vulkan_data);

#endif
