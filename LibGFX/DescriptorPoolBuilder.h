#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "VkContext.h"

namespace LibGFX {
	class DescriptorPoolBuilder
	{
	public:
		DescriptorPoolBuilder& addPoolSize(VkDescriptorType type, uint32_t count);
		DescriptorPoolBuilder& setMaxSets(uint32_t maxSets);
		VkDescriptorPool build(VkContext& context);
		void clear();
	private:
		std::vector<VkDescriptorPoolSize> m_poolSizes;
		uint32_t m_maxSets = 0;
	};
}
