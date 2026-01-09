#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {
	class RenderPass {
		public:
			RenderPass() = default;
			virtual ~RenderPass() = default;
			virtual VkRenderPass getRenderPass() const = 0;
			virtual bool create(VkDevice device, VkFormat swapchainImageFormat, VkFormat depthFormat) = 0;
			virtual void destroy(VkDevice device) = 0;
	};
}