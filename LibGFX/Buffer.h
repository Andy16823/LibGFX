#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {
	struct Buffer {
		VkBuffer buffer;
		VkDeviceMemory memory;
		VkDeviceSize size;
	};
}