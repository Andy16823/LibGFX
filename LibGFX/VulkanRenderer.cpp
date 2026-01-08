#include "VulkanRenderer.h"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace LibGFX;

VulkanRenderer::VulkanRenderer(GLFWwindow* targetWindow)
{
	m_targetWindow = targetWindow;
}

LibGFX::VulkanRenderer::~VulkanRenderer()
{
	m_targetWindow = nullptr;
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
			if (strcmp(reqLayer, layerProperties.layerName)) {
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
			if (strcmp(reqExt, ext.extensionName)) {
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

void VulkanRenderer::initialize()
{
	std::cout << "Initializing Vulkan Renderer..." << std::endl;
	std::cout << "Creating Validation Layers..." << std::endl;

	std::vector<const char*> layers;
	layers.push_back("VK_LAYER_KHRONOS_validation");
	if (!this->hasRequiredLayers(layers)) {
		throw std::runtime_error("Required validation layers not available");
	}

	// Create application info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "LibGFX";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "LibGFX";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_1;

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
}
