#include "VulkanRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <set>

using namespace LibGFX;

VulkanRenderer::VulkanRenderer(GLFWwindow* targetWindow)
{
	m_targetWindow = targetWindow;
}

LibGFX::VulkanRenderer::~VulkanRenderer()
{
	m_targetWindow = nullptr;
}

void VulkanRenderer::destroySwapChain(SwapchainInfo& swapchainInfo)
{
	for (auto imageView : swapchainInfo.imageViews) {
		vkDestroyImageView(m_device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(m_device, swapchainInfo.swapchain, nullptr);
}

VkImageView VulkanRenderer::createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType /*= VK_IMAGE_VIEW_TYPE_2D*/)
{
	VkImageViewCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = image;
	createInfo.viewType = viewType;
	createInfo.format = format;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.subresourceRange.aspectMask = aspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create image view");
	}
	return imageView;
}

VkExtent2D VulkanRenderer::chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}

	int width, height;
	glfwGetFramebufferSize(m_targetWindow, &width, &height);
	VkExtent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	};
	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

	return actualExtent;
}

bool VulkanRenderer::isPresentModeAvailable(VkPresentModeKHR presentMode)
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);
	for (const auto& availablePresentMode : swapChainSupport.presentModes) {
		if (availablePresentMode == presentMode) {
			return true;
		}
	}
	return false;
}

VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
		else if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

SwapchainInfo VulkanRenderer::createSwapChain(VkPresentModeKHR desiredPresentMode)
{
	// Check if desired present mode is available
	if (!this->isPresentModeAvailable(desiredPresentMode)) {
		throw std::runtime_error("Desired present mode is not available");
	}

	// TODO: Create isPresentModeAvailable public helper method
	SwapchainInfo swapchainInfo;

	// Get swap chain support details
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);

	// Choose swap surface format, present mode and extent
	swapchainInfo.surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	swapchainInfo.extent = chooseSwapchainExtent(swapChainSupport.capabilities);
	swapchainInfo.imageCount = swapChainSupport.capabilities.minImageCount + 1;

	// Set image count within allowed limits
	if(swapChainSupport.capabilities.maxImageCount > 0 && swapChainSupport.capabilities.maxImageCount < swapchainInfo.imageCount) {
		swapchainInfo.imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Create swap chain info
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_surface;
	createInfo.imageFormat = swapchainInfo.surfaceFormat.format;
	createInfo.imageColorSpace = swapchainInfo.surfaceFormat.colorSpace;
	createInfo.presentMode = desiredPresentMode;
	createInfo.imageExtent = swapchainInfo.extent;
	createInfo.minImageCount = swapchainInfo.imageCount;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.clipped = VK_TRUE;

	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
	if (!indices.gpShared()) {
		uint32_t queueFamilyIndices[] = {
			static_cast<uint32_t>(indices.graphicsFamily),
			static_cast<uint32_t>(indices.presentFamily)
		};
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &swapchainInfo.swapchain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap chain");
	}

	// Get swap chain images
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(m_device, swapchainInfo.swapchain, &imageCount, nullptr);

	swapchainInfo.images.resize(imageCount);
	
	vkGetSwapchainImagesKHR(m_device, swapchainInfo.swapchain, &imageCount, swapchainInfo.images.data());

	swapchainInfo.imageViews.resize(imageCount);
	for (size_t i = 0; i < imageCount; i++) {
		swapchainInfo.imageViews[i] = createImageView(m_device, swapchainInfo.images[i], swapchainInfo.surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
	}
	return swapchainInfo;
}

void VulkanRenderer::dispose()
{
	if (m_device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(m_device);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}
}

LibGFX::SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	if (extensionCount == 0) {
		return false;
	}

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	for (const auto& reqExt : deviceExtensions) {
		bool hasExtension = false;
		for (const auto& ext : availableExtensions) {
			if (strcmp(reqExt, ext.extensionName) == 0) {
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension) {
			return false;
		}
	}
	return true;
}

LibGFX::QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilies.size(); i++) {
		auto& queueFamily = queueFamilies[i];

		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = static_cast<int>(i);
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = static_cast<int>(i);
		}

		if (indices.isValid())
		{
			break;
		}
	}
	return indices;
}

bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions)
{
	// Check for required features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Check for required queue families (graphics and present, is mostly the same)
	QueueFamilyIndices indices = findQueueFamilies(device);

	// Check for required DEVICE EXTENSIONS not instance extensions 
	bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

	// Check for swap chain support
	SwapChainSupportDetails swapChainDetails = querySwapChainSupport(device);
	bool swapChainAdequate = swapChainDetails.isValid();

	// Final suitability check
	return indices.isValid() && extensionsSupported && swapChainAdequate && deviceFeatures.samplerAnisotropy;
}

VkPhysicalDevice VulkanRenderer::selectPhysicalDevice(const std::vector<const char*> deviceExtensions)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

	if(deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if(isDeviceSuitable(device, deviceExtensions)) {
			return device;
		}
	}

	return VK_NULL_HANDLE;
}

VkApplicationInfo VulkanRenderer::defaultAppInfo()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "LibGFX";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "LibGFX";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;
	return appInfo;
}

bool VulkanRenderer::hasRequiredLayers(const std::vector<const char*> requiredLayers)
{
	// Get the layers count
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Get the layer info
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Check for the KHR_layer
	for (const auto& reqLayer : requiredLayers) {
		bool result = false;
		for (const auto& layerProperties : availableLayers) {
			if (strcmp(reqLayer, layerProperties.layerName) == 0) {
				result = true;
				break;
			}
		}
		if (!result) {
			return false;
		}
	}
	return true;
}

bool VulkanRenderer::hasRequiredExtensions(const std::vector<const char*>* requiredExtensions)
{
	// Get the available extensions count
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// Get the available extension properties
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

	// Check for the required extensions if they exist
	for (const auto& reqExt : *requiredExtensions) {
		bool result = false;
		for (const auto& ext : availableExtensions) {
			if (strcmp(reqExt, ext.extensionName) == 0) {
				result = true;
				break;
			}
		}
		if (!result) {
			return false;
		}
	}
	return true;
}

void VulkanRenderer::initialize(VkApplicationInfo appInfo)
{
	std::cout << "Initializing Vulkan Renderer..." << std::endl;
	std::cout << "Creating Validation Layers..." << std::endl;

	std::vector<const char*> layers;
	layers.push_back("VK_LAYER_KHRONOS_validation");
	if (!this->hasRequiredLayers(layers)) {
		throw std::runtime_error("Required validation layers not available");
	}

	// Create instance info
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Get required extensions
	std::vector<const char*> extensions;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (size_t i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if (!this->hasRequiredExtensions(&extensions)) {
		throw std::runtime_error("Required Vulkan features are not available");
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = layers.data();

	if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to initialize Vulkan");
	}
	std::cout << "Vulkan instance created successfully!" << std::endl;

	// Create Surface
	std::cout << "Creating Vulkan Surface..." << std::endl;
	if (glfwCreateWindowSurface(m_instance, m_targetWindow, nullptr, &m_surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Vulkan surface");
	}

	// Get Physical Device
	std::cout << "Selecting Physical Device..." << std::endl;

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	m_physicalDevice = selectPhysicalDevice(deviceExtensions);
	if (m_physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find a suitable GPU");
	}

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
	std::cout << "Selected GPU: " << deviceProperties.deviceName << std::endl;

	// Create Logical Device
	QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

	for (int queueFamilyIndex : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = static_cast<uint32_t>(queueFamilyIndex);
		queueCreateInfo.queueCount = 1;
		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

	if (vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	std::cout << "Vulkan Logical Device created successfully!" << std::endl;

	// Get queues
	vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);
}
