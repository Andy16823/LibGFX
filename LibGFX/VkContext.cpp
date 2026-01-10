#include "VkContext.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <set>

using namespace LibGFX;

VkContext::VkContext(GLFWwindow* targetWindow)
{
	m_targetWindow = targetWindow;
}

LibGFX::VkContext::~VkContext()
{
	m_targetWindow = nullptr;
}

void VkContext::presentImage(VkQueue presentQueue, const VkPresentInfoKHR& presentInfo)
{
	if (vkQueuePresentKHR(presentQueue, &presentInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to present image");
	}
}

void VkContext::submitCommandBuffers(const std::vector<VkSubmitInfo>& submitInfos, VkFence fence /*= VK_NULL_HANDLE*/)
{
	if (vkQueueSubmit(m_graphicsQueue, static_cast<uint32_t>(submitInfos.size()), submitInfos.data(), fence) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit command buffers");
	}
}

void VkContext::submitCommandBuffer(const VkSubmitInfo& submitInfo, VkFence fence /*= VK_NULL_HANDLE*/)
{
	if (vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, fence) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit command buffer");
	}
}

void VkContext::endCommandBuffer(VkCommandBuffer commandBuffer)
{
	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer");
	}
}

void VkContext::endRenderPass(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

void VkContext::bindPipeline(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, const Pipeline& pipeline)
{
	vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline.getPipeline());
}

void VkContext::beginRenderPass(VkCommandBuffer commandBuffer, const RenderPass& renderPass, VkFramebuffer framebuffer, VkExtent2D extent, VkSubpassContents contents)
{
	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass = renderPass.getRenderPass();
	beginInfo.framebuffer = framebuffer;
	beginInfo.renderArea.offset = { 0, 0 };
	beginInfo.renderArea.extent = extent;
	auto clearValues = renderPass.getClearValues();
	beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	beginInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &beginInfo, contents);
}

void VkContext::beginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags flags /*= 0*/)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = flags;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording command buffer");
	}
}

void VkContext::resetFence(VkFence fence)
{
	vkResetFences(m_device, 1, &fence);
}

VkResult VkContext::acquireNextImage(const SwapchainInfo& swapchainInfo, VkSemaphore signalSemaphore, VkFence fence, uint32_t& imageIndex, uint64_t timeout)
{
	VkResult result = vkAcquireNextImageKHR(
		m_device,
		swapchainInfo.swapchain,
		timeout,
		signalSemaphore,
		fence,
		&imageIndex);

	return result;
}

void VkContext::waitForFence(VkFence fence, uint64_t timeout /*= std::numeric_limits<uint64_t>::max()*/)
{
	vkWaitForFences(m_device, 1, &fence, VK_TRUE, timeout);
}

void VkContext::destroyFences(std::vector<VkFence>& fences)
{
	for (auto& fence : fences) {
		vkDestroyFence(m_device, fence, nullptr);
	}
}

void VkContext::destroySemaphores(std::vector<VkSemaphore>& semaphores)
{
	for (auto& semaphore : semaphores) {
		vkDestroySemaphore(m_device, semaphore, nullptr);
	}
}

void VkContext::destroyFence(VkFence& fence)
{
	vkDestroyFence(m_device, fence, nullptr);
}

std::vector<VkFence> VkContext::createFences(uint32_t count, VkFenceCreateFlags flags /*= 0*/)
{
	std::vector<VkFence> fences(count);
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = flags;

	for (uint32_t i = 0; i < count; i++) {
		if (vkCreateFence(m_device, &fenceInfo, nullptr, &fences[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create fence");
		}
	}
	return fences;
}

VkFence VkContext::createFence(VkFenceCreateFlags flags /*= 0*/)
{
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = flags;

	VkFence fence;
	if (vkCreateFence(m_device, &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create fence");
	}
	return fence;
}

void VkContext::destroySemaphore(VkSemaphore& semaphore)
{
	vkDestroySemaphore(m_device, semaphore, nullptr);
}

std::vector<VkSemaphore> VkContext::createSemaphores(uint32_t count)
{
	std::vector<VkSemaphore> semaphores(count);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	for (uint32_t i = 0; i < count; i++) {
		if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &semaphores[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create semaphore");
		}
	}
	return semaphores;
}

VkSemaphore VkContext::createSemaphore()
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	
	VkSemaphore semaphore;
	if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create semaphore");
	}
	return semaphore;
}

void VkContext::destroyDescriptorSetPool(VkDescriptorPool& descriptorPool)
{
	vkDestroyDescriptorPool(m_device, descriptorPool, nullptr);
}

VkSampler VkContext::createCubeMapSampler(bool enableAnisotropy, float maxAnisotropy)
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;
	if (!enableAnisotropy) {
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;
	}
	else
	{
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = maxAnisotropy;
	}

	return createSampler(samplerInfo);
}

VkSampler VkContext::createTextureSampler(bool enableAnisotropy, float maxAnisotropy)
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;
	if (!enableAnisotropy) {
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;
	}
	else
	{
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = maxAnisotropy;
	}
	return createSampler(samplerInfo);
}

void VkContext::destroySampler(VkSampler& sampler)
{
	vkDestroySampler(m_device, sampler, nullptr);
}

VkSampler VkContext::createSampler(const VkSamplerCreateInfo& createInfo)
{
	VkSampler sampler;
	if (vkCreateSampler(m_device, &createInfo, nullptr, &sampler) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create texture sampler");
	}
	return sampler;
}

void VkContext::freeCommandBuffers(VkCommandPool commandPool, std::vector<VkCommandBuffer>& commandBuffers)
{
	vkFreeCommandBuffers(m_device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

void VkContext::freeCommandBuffer(VkCommandPool commandPool, VkCommandBuffer& commandBuffer)
{
	vkFreeCommandBuffers(m_device, commandPool, 1, &commandBuffer);
}

std::vector<VkCommandBuffer> VkContext::allocateCommandBuffers(VkCommandPool commandPool, uint32_t count, VkCommandBufferLevel level /*= VK_COMMAND_BUFFER_LEVEL_PRIMARY*/)
{
	std::vector<VkCommandBuffer> commandBuffers(count);

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = level;
	allocInfo.commandBufferCount = count;

	if (vkAllocateCommandBuffers(m_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers");
	}

	return commandBuffers;
}

VkCommandBuffer VkContext::allocateCommandBuffer(VkCommandPool commandPool, VkCommandBufferLevel level /*= VK_COMMAND_BUFFER_LEVEL_PRIMARY*/)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = level;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	if (vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffer");
	}
	return commandBuffer;
}

void VkContext::destroyCommandPool(VkCommandPool& commandPool)
{
	vkDestroyCommandPool(m_device, commandPool, nullptr);
}

VkCommandPool VkContext::createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags /*= 0*/)
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndex;
	poolInfo.flags = flags;

	VkCommandPool commandPool;
	if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool");
	}
	return commandPool;
}

void VkContext::destroyFramebuffer(VkFramebuffer& framebuffer)
{
	vkDestroyFramebuffer(m_device, framebuffer, nullptr);
}

std::vector<VkFramebuffer> VkContext::createFramebuffers(RenderPass& renderPass, const SwapchainInfo& swapchainInfo)
{
	std::vector<VkFramebuffer> framebuffers;
	framebuffers.resize(swapchainInfo.imageViews.size());
	for (size_t i = 0; i < swapchainInfo.imageViews.size(); i++) {
		framebuffers[i] = createFramebuffer(renderPass, swapchainInfo.imageViews[i], swapchainInfo.extent);
	}
	return framebuffers;
}

std::vector<VkFramebuffer> VkContext::createFramebuffers(RenderPass& renderPass, const SwapchainInfo& swapchainInfo, DepthBuffer depthBuffer)
{
	std::vector<VkFramebuffer> framebuffers;
	framebuffers.resize(swapchainInfo.imageViews.size());
	for (size_t i = 0; i < swapchainInfo.imageViews.size(); i++) {
		framebuffers[i] = createFramebuffer(renderPass, swapchainInfo.imageViews[i], depthBuffer, swapchainInfo.extent);
	}
	return framebuffers;
}

VkFramebuffer VkContext::createFramebuffer(RenderPass& renderPass, VkImageView imageView, VkExtent2D extent)
{
	std::array<VkImageView, 1> attachments = {
		imageView
	};

	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass.getRenderPass();
	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebufferInfo.pAttachments = attachments.data();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;

	VkFramebuffer framebuffer;
	if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create framebuffer");
	}
	
	return framebuffer;
}

VkFramebuffer VkContext::createFramebuffer(RenderPass& renderPass, VkImageView imageView, DepthBuffer depthBuffer, VkExtent2D extent)
{
	std::array<VkImageView, 2> attachments = {
		imageView,
		depthBuffer.imageView
	};

	VkFramebufferCreateInfo framebufferInfo = {};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass.getRenderPass();
	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	framebufferInfo.pAttachments = attachments.data();
	framebufferInfo.width = extent.width;
	framebufferInfo.height = extent.height;
	framebufferInfo.layers = 1;

	VkFramebuffer framebuffer;
	if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create framebuffer");
	}
	
	return framebuffer;
}

void VkContext::destroyShaderModule(VkShaderModule shaderModule)
{
	vkDestroyShaderModule(m_device, shaderModule, nullptr);
}

VkRect2D VkContext::createScissorRect(int32_t offsetX, int32_t offsetY, VkExtent2D extent)
{
	VkRect2D scissor = {};
	scissor.offset = { offsetX, offsetY };
	scissor.extent = extent;
	return scissor;
}

VkViewport VkContext::createViewport(float x, float y, VkExtent2D extent, float minDepth /*= 0.0f*/, float maxDepth /*= 1.0f*/)
{
	VkViewport viewport = {};
	viewport.x = x;
	viewport.y = y;
	viewport.width = static_cast<float>(extent.width);
	viewport.height = static_cast<float>(extent.height);
	viewport.minDepth = minDepth;
	viewport.maxDepth = maxDepth;
	return viewport;
}

VkShaderModule VkContext::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create shader module");
	}
	return shaderModule;
}

void VkContext::destroyDescriptorSetLayout(VkDescriptorSetLayout descriptorSetLayout)
{
	vkDestroyDescriptorSetLayout(m_device, descriptorSetLayout, nullptr);
}

void VkContext::destroyDepthBuffer(DepthBuffer& depthBuffer)
{
	vkDestroyImageView(m_device, depthBuffer.imageView, nullptr);
	vkDestroyImage(m_device, depthBuffer.image, nullptr);
	vkFreeMemory(m_device, depthBuffer.memory, nullptr);
}

VkFormat VkContext::findSuitableDepthFormat()
{
	auto format = selectSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	return format;
}

VkFormat VkContext::selectSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	return VK_FORMAT_UNDEFINED;
}

uint32_t VkContext::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}
	throw std::runtime_error("Failed to find suitable memory type");
}

VkImage VkContext::createImage(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory* imageMemory)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage image;
	if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create image");
	}

	// Allocate memory for the image
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate image memory");
	}

	vkBindImageMemory(device, image, *imageMemory, 0);

	return image;
}

LibGFX::DepthBuffer VkContext::createDepthBuffer(VkExtent2D extent, VkFormat format)
{
	if (format == VK_FORMAT_UNDEFINED) {
		throw std::runtime_error("Failed to find supported depth format");
	}

	// Create depth image
	VkDeviceMemory depthImageMemory;
	VkImage depthImage = createImage(
		m_physicalDevice,
		m_device,
		extent.width,
		extent.height,
		format,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&depthImageMemory);

	// Create depth image view
	VkImageView depthImageView = createImageView(
		m_device, 
		depthImage, 
		format, 
		VK_IMAGE_ASPECT_DEPTH_BIT);

	// Create depth buffer struct
	DepthBuffer depthBuffer = {};
	depthBuffer.format = format;
	depthBuffer.image = depthImage;
	depthBuffer.memory = depthImageMemory;
	depthBuffer.imageView = depthImageView;
	return depthBuffer;
}

void VkContext::destroySwapChain(SwapchainInfo& swapchainInfo)
{
	for (auto imageView : swapchainInfo.imageViews) {
		vkDestroyImageView(m_device, imageView, nullptr);
	}
	vkDestroySwapchainKHR(m_device, swapchainInfo.swapchain, nullptr);
}

VkImageView VkContext::createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType /*= VK_IMAGE_VIEW_TYPE_2D*/)
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

VkExtent2D VkContext::chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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

bool VkContext::isPresentModeAvailable(VkPresentModeKHR presentMode)
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);
	for (const auto& availablePresentMode : swapChainSupport.presentModes) {
		if (availablePresentMode == presentMode) {
			return true;
		}
	}
	return false;
}

VkSurfaceFormatKHR VkContext::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

SwapchainInfo VkContext::createSwapChain(VkPresentModeKHR desiredPresentMode)
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

	QueueFamilyIndices indices = getQueueFamilyIndices(m_physicalDevice);
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

void VkContext::dispose()
{
	if (m_device != VK_NULL_HANDLE) {
		vkDeviceWaitIdle(m_device);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyDevice(m_device, nullptr);
		vkDestroyInstance(m_instance, nullptr);
	}
}

LibGFX::SwapChainSupportDetails VkContext::querySwapChainSupport(VkPhysicalDevice device)
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

bool VkContext::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions)
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

LibGFX::QueueFamilyIndices VkContext::getQueueFamilyIndices(VkPhysicalDevice device)
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

bool VkContext::isDeviceSuitable(VkPhysicalDevice device, const std::vector<const char*> deviceExtensions)
{
	// Check for required features
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Check for required queue families (graphics and present, is mostly the same)
	QueueFamilyIndices indices = getQueueFamilyIndices(device);

	// Check for required DEVICE EXTENSIONS not instance extensions 
	bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

	// Check for swap chain support
	SwapChainSupportDetails swapChainDetails = querySwapChainSupport(device);
	bool swapChainAdequate = swapChainDetails.isValid();

	// Final suitability check
	return indices.isValid() && extensionsSupported && swapChainAdequate && deviceFeatures.samplerAnisotropy;
}

VkPhysicalDevice VkContext::selectPhysicalDevice(const std::vector<const char*> deviceExtensions)
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

VkApplicationInfo VkContext::defaultAppInfo()
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

bool VkContext::hasRequiredLayers(const std::vector<const char*> requiredLayers)
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

bool VkContext::hasRequiredExtensions(const std::vector<const char*>* requiredExtensions)
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

void VkContext::initialize(VkApplicationInfo appInfo)
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
	QueueFamilyIndices indices = getQueueFamilyIndices(m_physicalDevice);

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
