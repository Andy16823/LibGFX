#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "QueueFamilyIndices.h"
#include "SwapchainSupportDetails.h"
#include "SwapchainInfo.h"
#include "DepthBuffer.h"
#include "RenderPass.h"

namespace LibGFX {
	class VkContext {
	public:
		VkContext(GLFWwindow* targetWindow);
		~VkContext();

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

		// Framebuffer functions
		VkFramebuffer createFramebuffer(RenderPass& renderPass, VkImageView imageView, DepthBuffer depthBuffer, VkExtent2D extent);
		VkFramebuffer createFramebuffer(RenderPass& renderPass, VkImageView imageView, VkExtent2D extent);
		std::vector<VkFramebuffer> createFramebuffers(RenderPass& renderPass, const SwapchainInfo& swapchainInfo, DepthBuffer depthBuffer);
		std::vector<VkFramebuffer> createFramebuffers(RenderPass& renderPass, const SwapchainInfo& swapchainInfo);
		void destroyFramebuffer(VkFramebuffer& framebuffer);

		// Command pool functions
		VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
		void destroyCommandPool(VkCommandPool& commandPool);

		// Command buffer functions
		VkCommandBuffer allocateCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		std::vector<VkCommandBuffer> allocateCommandBuffers(VkCommandPool commandPool, uint32_t count, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		void freeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer& commandBuffer);
		void freeCommandBuffers(VkCommandPool commandPool, std::vector<VkCommandBuffer>& commandBuffers);

		// Sampler functions
		VkSampler createSampler(const VkSamplerCreateInfo& createInfo);
		VkSampler createTextureSampler(bool enableAnisotropy = true, float maxAnisotropy = 16.0f);
		VkSampler createCubeMapSampler(bool enableAnisotropy = true, float maxAnisotropy = 16.0f);
		void destroySampler(VkSampler& sampler);

		// Descriptor set functions
		void destroyDescriptorSetPool(VkDescriptorPool& descriptorPool);

		// Semaphore & Fence functions
		VkSemaphore createSemaphore();
		std::vector<VkSemaphore> createSemaphores(uint32_t count);
		void destroySemaphore(VkSemaphore& semaphore);
		void destroySemaphores(std::vector<VkSemaphore>& semaphores);

		VkFence createFence(VkFenceCreateFlags flags = 0);
		std::vector<VkFence> createFences(uint32_t count, VkFenceCreateFlags flags = 0);
		void destroyFence(VkFence& fence);
		void destroyFences(std::vector<VkFence>& fences);

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
		QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice device);
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
		bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		// Swapchain helpers
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkExtent2D chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		// Depth buffer helpers
		

		GLFWwindow* m_targetWindow;
	};
}
