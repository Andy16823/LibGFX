#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include "VkRenderer.h"

namespace LibGFX {
	class GFX {
	public:
		static GLFWwindow* createContext(int width, int height, const char* title);
		static std::unique_ptr<VkRenderer> createVulkanRenderer(GLFWwindow* targetWindow) {
			return std::make_unique<VkRenderer>(targetWindow);
		}
	};
}

