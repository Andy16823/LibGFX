#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>

namespace LibGFX {
	class GFX {
	public:
		GLFWwindow* createContext(int width, int height, const char* title);
		static std::unique_ptr<GFX> create() {
			return std::make_unique<GFX>();
		}
	};
}

