#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VkContext.h"

namespace LibGFX {
	class DescriptorSetWriter
	{
	public:
		DescriptorSetWriter& addBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);
		DescriptorSetWriter& addImageInfo(VkImageView imageView, VkSampler sampler, VkImageLayout imageLayout);
		DescriptorSetWriter& write(VkContext& context, VkDescriptorSet descriptorSet, uint32_t dstBinding, uint32_t dstArrayElement, VkDescriptorType descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		void clear();
	private:
		std::vector<VkDescriptorBufferInfo> m_bufferInfos;
		std::vector<VkDescriptorImageInfo> m_imageInfos;
	};
}
