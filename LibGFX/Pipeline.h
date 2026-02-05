#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {

	// Forward declaration of VkContext to avoid circular dependency
	class VkContext;

	// Abstract base class for a graphics pipeline
	class Pipeline {

	public:
		Pipeline() = default;
		virtual ~Pipeline() = default;

		virtual void create(VkContext* context, VkRenderPass renderPass) = 0;
		virtual void destroy(VkContext* context) = 0;
		virtual VkPipeline getPipeline() const = 0;
		virtual VkPipelineLayout getPipelineLayout() const = 0;
	};
}
