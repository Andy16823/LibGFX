#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueFamilyIndices.h"
#include "SwapchainSupportDetails.h"
#include "SwapchainInfo.h"

namespace LibGFX {
	class VulkanRenderer {
	public:
		VulkanRenderer(GLFWwindow* targetWindow);
		~VulkanRenderer();

		static VkApplicationInfo defaultAppInfo();

		void initialize(VkApplicationInfo appInfo);
		SwapchainInfo createSwapChain(VkPresentModeKHR desiredPresentMode);
		void destroySwapChain(SwapchainInfo& swapchainInfo);
		void dispose();

		// Public Helpers
		bool isPresentModeAvailable(VkPresentModeKHR presentMode);
		static VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);


	private:
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

		// Initialization helpers
		bool hasRequiredLayers(const std::vector<const char*> requiredLayers);
		bool hasRequiredExtensions(const std::vector<const char*>* requiredExtensions);
		VkPhysicalDevice selectPhysicalDevice(const std::vector<const char*> deviceExtensions);
		bool isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		// Swapchain helpers
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		GLFWwindow* m_targetWindow;
	};
}
