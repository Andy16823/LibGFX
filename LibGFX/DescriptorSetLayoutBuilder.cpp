#include "DescriptorSetLayoutBuilder.h"
#include <stdexcept>

LibGFX::DescriptorSetLayoutBuilder& LibGFX::DescriptorSetLayoutBuilder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t descriptorCount /*= 1*/)
{
	DescriptorBindingInfo bindingInfo = {};
	bindingInfo.binding = binding;
	bindingInfo.descriptorType = descriptorType;
	bindingInfo.stageFlags = stageFlags;
	bindingInfo.descriptorCount = descriptorCount;
	m_bindings.push_back(bindingInfo);	
	return *this;
}

VkDescriptorSetLayout LibGFX::DescriptorSetLayoutBuilder::build(VkContext& context) 
{
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	layoutBindings.reserve(m_bindings.size());

	for (const auto& bindingInfo : m_bindings) {
		VkDescriptorSetLayoutBinding layoutBinding = {};
		layoutBinding.binding = bindingInfo.binding;
		layoutBinding.descriptorType = bindingInfo.descriptorType;	
		layoutBinding.descriptorCount = bindingInfo.descriptorCount;
		layoutBinding.stageFlags = bindingInfo.stageFlags;
		layoutBinding.pImmutableSamplers = nullptr;
		layoutBindings.push_back(layoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;	
	layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
	layoutCreateInfo.pBindings = layoutBindings.data();

	VkDescriptorSetLayout descriptorSetLayout;
	if (vkCreateDescriptorSetLayout(context.getDevice(), &layoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor set layout");
	}

	return descriptorSetLayout;
}
