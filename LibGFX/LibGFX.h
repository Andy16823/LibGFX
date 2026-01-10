#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include "VkContext.h"

namespace LibGFX {
	class GFX {
	public:
		static GLFWwindow* createWindow(int width, int height, const char* title);
		static std::vector<char> readFile(const std::string& filename);
		static std::unique_ptr<VkContext> createContext(GLFWwindow* targetWindow) {
			return std::make_unique<VkContext>(targetWindow);
		}
	};
}

