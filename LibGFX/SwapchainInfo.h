#pragma once
#include <vulkan/vulkan.h>
#include <vector>

struct SwapchainInfo {
	VkSwapchainKHR swapchain;
	VkExtent2D extent;
	VkSurfaceFormatKHR surfaceFormat;
	uint32_t imageCount;
	std::vector<VkImage> images;
	std::vector<VkImageView> imageViews;
};