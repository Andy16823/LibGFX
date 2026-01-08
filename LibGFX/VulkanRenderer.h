#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace LibGFX {
	class VulkanRenderer {
	public:
		VulkanRenderer(GLFWwindow* targetWindow);
		~VulkanRenderer();
	private:
		GLFWwindow* m_targetWindow;
	};
}
