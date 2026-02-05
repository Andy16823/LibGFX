#pragma once
#include <vulkan/vulkan.h>
#include <span>

namespace LibGFX {

	// Forward declaration of VkContext to avoid circular dependency
	class VkContext;

	// Abstract base class for a render pass
	class RenderPass {
		public:
			RenderPass() = default;
			virtual ~RenderPass() = default;
			virtual VkRenderPass getRenderPass() const = 0;
			virtual std::span<const VkClearValue> getClearValues() const = 0;
			virtual bool create(VkContext& context, VkFormat swapchainImageFormat, VkFormat depthFormat) = 0;
			virtual void destroy(VkContext& context) = 0;
	};
}