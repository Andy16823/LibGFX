#pragma once
#include <vulkan/vulkan.h>
#include <vector>

namespace LibGFX {
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		bool isValid() {
			return !formats.empty() && !presentModes.empty();
		}
	};
}