#pragma once
#include <vulkan/vulkan.h>
#include <vector>

// Info about a single descriptor binding
struct DescriptorBindingInfo
{
	uint32_t binding;
	VkDescriptorType descriptorType;
	uint32_t descriptorCount = 1;
	VkShaderStageFlags stageFlags;
};

namespace LibGFX {
	// Builder class for creating descriptor set layouts
	class DescriptorSetLayoutBuilder
	{
	public:
		DescriptorSetLayoutBuilder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount = 1);
		VkDescriptorSetLayout build(VkDevice device);
		void clear() { m_bindings.clear(); }
	private:
		std::vector<DescriptorBindingInfo> m_bindings;
	};
}
