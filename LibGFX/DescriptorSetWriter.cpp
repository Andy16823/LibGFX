#include "DescriptorSetWriter.h"

LibGFX::DescriptorSetWriter& LibGFX::DescriptorSetWriter::addBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range)
{
	VkDescriptorBufferInfo bufferInfo = {};
	bufferInfo.buffer = buffer;
	bufferInfo.offset = offset;
	bufferInfo.range = range;
	m_bufferInfos.push_back(bufferInfo);
	return *this;
}

LibGFX::DescriptorSetWriter& LibGFX::DescriptorSetWriter::setDstBinding(uint32_t dstBinding)
{
	m_dstBinding = dstBinding;
	return *this;
}

LibGFX::DescriptorSetWriter& LibGFX::DescriptorSetWriter::setDstArrayElement(uint32_t dstArrayElement)
{
	m_dstArrayElement = dstArrayElement;
	return *this;
}

LibGFX::DescriptorSetWriter& LibGFX::DescriptorSetWriter::setDescriptorType(VkDescriptorType descriptorType)
{
	m_descriptorType = descriptorType;
	return *this;
}

void LibGFX::DescriptorSetWriter::write(VkContext& context, VkDescriptorSet descriptorSet)
{
	VkWriteDescriptorSet writeInfo = {};
	writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeInfo.dstSet = descriptorSet;
	writeInfo.dstBinding = m_dstBinding;
	writeInfo.dstArrayElement = m_dstArrayElement;
	writeInfo.descriptorCount = 1;
	writeInfo.descriptorType = m_descriptorType;
	writeInfo.pBufferInfo = m_bufferInfos.data();

	vkUpdateDescriptorSets(context.getDevice(), 1, &writeInfo, 0, nullptr);
}

void LibGFX::DescriptorSetWriter::clear()
{
	m_bufferInfos.clear();
	m_dstBinding = 0;
	m_dstArrayElement = 0;
	m_descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}
