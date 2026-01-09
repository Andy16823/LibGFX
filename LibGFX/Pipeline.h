#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {

	// Abstract base class for a Vulkan pipeline
	class Pipeline {
		public:
		Pipeline() = default;
		virtual ~Pipeline() = default;

		virtual void create(VkDevice device, VkRenderPass renderPass, VkViewport viewport, VkRect2D scissor) = 0;
		virtual void destroy(VkDevice device) = 0;
		virtual VkPipeline getPipeline() const = 0;
	};
}
