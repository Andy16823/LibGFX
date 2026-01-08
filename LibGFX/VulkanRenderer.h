#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace LibGFX {
	class VulkanRenderer {
	public:
		VulkanRenderer(GLFWwindow* targetWindow);
		~VulkanRenderer();

		void initialize();

	private:
		VkInstance m_instance;

		bool hasRequiredLayers(const std::vector<const char*> requiredLayers);
		bool hasRequiredExtensions(const std::vector<const char*>* requiredExtensions);

		GLFWwindow* m_targetWindow;
	};
}
