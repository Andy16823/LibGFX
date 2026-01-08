#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueFamilyIndices.h"
#include "SwapchainSupportDetails.h"

namespace LibGFX {
	class VulkanRenderer {
	public:
		VulkanRenderer(GLFWwindow* targetWindow);
		~VulkanRenderer();

		static VkApplicationInfo defaultAppInfo();

		void initialize(VkApplicationInfo appInfo);
		void dispose();

	private:
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		bool hasRequiredLayers(const std::vector<const char*> requiredLayers);
		bool hasRequiredExtensions(const std::vector<const char*>* requiredExtensions);
		VkPhysicalDevice selectPhysicalDevice(const std::vector<const char*> deviceExtensions);
		bool isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		GLFWwindow* m_targetWindow;
	};
}
