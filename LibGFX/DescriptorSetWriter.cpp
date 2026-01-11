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

LibGFX::DescriptorSetWriter& LibGFX::DescriptorSetWriter::addImageInfo(VkImageView imageView, VkSampler sampler, VkImageLayout imageLayout)
{
	VkDescriptorImageInfo imageInfo = {};
	imageInfo.imageView = imageView;
	imageInfo.sampler = sampler;
	imageInfo.imageLayout = imageLayout;
	m_imageInfos.push_back(imageInfo);
	return *this;
}

LibGFX::DescriptorSetWriter& LibGFX::DescriptorSetWriter::write(VkContext& context, VkDescriptorSet descriptorSet, uint32_t dstBinding, uint32_t dstArrayElement, VkDescriptorType descriptorType)
{
	VkWriteDescriptorSet writeInfo = {};
	writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeInfo.dstSet = descriptorSet;
	writeInfo.dstBinding = dstBinding;
	writeInfo.dstArrayElement = dstArrayElement;
	writeInfo.descriptorCount = 1;
	writeInfo.descriptorType = descriptorType;
	writeInfo.pBufferInfo = m_bufferInfos.data();
	writeInfo.pImageInfo = m_imageInfos.data();

	vkUpdateDescriptorSets(context.getDevice(), 1, &writeInfo, 0, nullptr);
	return *this;
}

void LibGFX::DescriptorSetWriter::clear()
{
	m_bufferInfos.clear();
	m_imageInfos.clear();
}
