#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VkContext.h"

namespace LibGFX {
	class DescriptorSetWriter
	{
	public:
		DescriptorSetWriter& addBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);
		DescriptorSetWriter& setDstBinding(uint32_t dstBinding);
		DescriptorSetWriter& setDstArrayElement(uint32_t dstArrayElement);
		DescriptorSetWriter& setDescriptorType(VkDescriptorType descriptorType);
		void write(VkContext& context, VkDescriptorSet descriptorSet);
		void clear();
	private:
		std::vector<VkDescriptorBufferInfo> m_bufferInfos;
		uint32_t m_dstBinding = 0;
		uint32_t m_dstArrayElement = 0;
		VkDescriptorType m_descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	};
}
