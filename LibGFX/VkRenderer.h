#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueFamilyIndices.h"
#include "SwapchainSupportDetails.h"
#include "SwapchainInfo.h"
#include "DepthBuffer.h"

namespace LibGFX {
	class VkRenderer {
	public:
		VkRenderer(GLFWwindow* targetWindow);
		~VkRenderer();

		static VkApplicationInfo defaultAppInfo();

		void initialize(VkApplicationInfo appInfo);
		void dispose();

		// Swapchain functions
		SwapchainInfo createSwapChain(VkPresentModeKHR desiredPresentMode);
		void destroySwapChain(SwapchainInfo& swapchainInfo);
		
		// Public functions
		VkFormat selectSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		DepthBuffer createDepthBuffer(VkExtent2D extent, VkFormat format);
		void destroyDepthBuffer(DepthBuffer& depthBuffer);
		void destroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout);
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void destroyShaderModule(VkShaderModule shaderModule);

		// Getters
		VkInstance getInstance() const { return m_instance; }
		VkSurfaceKHR getSurface() const { return m_surface; }
		VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }
		VkDevice getDevice() const { return m_device; }
		VkQueue getGraphicsQueue() const { return m_graphicsQueue; }
		VkQueue getPresentQueue() const { return m_presentQueue; }

		// Public Helpers
		bool isPresentModeAvailable(VkPresentModeKHR presentMode);
		static VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
		static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static VkImage createImage(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory* imageMemory);
		static VkViewport createViewport(float x, float y, VkExtent2D extent, float minDepth = 0.0f, float maxDepth = 1.0f);
		static VkRect2D createScissorRect(int32_t offsetX, int32_t offsetY, VkExtent2D extent);
		VkFormat findSuitableDepthFormat();

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

		// Depth buffer helpers
		

		GLFWwindow* m_targetWindow;
	};
}
