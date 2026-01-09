#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {
	struct DepthBuffer {
		VkImage image;
		VkDeviceMemory memory;
		VkImageView imageView;
		VkFormat format;
	};
}