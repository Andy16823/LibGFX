#include "DescriptorPoolBuilder.h"
#include <stdexcept>


LibGFX::DescriptorPoolBuilder& LibGFX::DescriptorPoolBuilder::addPoolSize(VkDescriptorType type, uint32_t count)
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = type;
	poolSize.descriptorCount = count;
	m_poolSizes.push_back(poolSize);

	return *this;
}

LibGFX::DescriptorPoolBuilder& LibGFX::DescriptorPoolBuilder::setMaxSets(uint32_t maxSets)
{
	m_maxSets = maxSets;
	return *this;
}

VkDescriptorPool LibGFX::DescriptorPoolBuilder::build(VkContext& context)
{
	VkDescriptorPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.poolSizeCount = static_cast<uint32_t>(m_poolSizes.size());
	poolCreateInfo.pPoolSizes = m_poolSizes.data();
	poolCreateInfo.maxSets = m_maxSets;

	VkDescriptorPool descriptorPool;
	if (vkCreateDescriptorPool(context.getDevice(), &poolCreateInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create descriptor pool");
	}
	return descriptorPool;
}
