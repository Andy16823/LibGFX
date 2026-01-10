#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {

	class VkRenderer;

	class Pipeline {

	public:
		Pipeline() = default;
		virtual ~Pipeline() = default;

		virtual void create(VkRenderer* renderer, VkRenderPass renderPass, VkViewport viewport, VkRect2D scissor) = 0;
		virtual void destroy(VkRenderer* renderer) = 0;
		virtual VkPipeline getPipeline() const = 0;
	};
}
