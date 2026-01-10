#pragma once
#include <vulkan/vulkan.h>

namespace LibGFX {
	// Abstract base class for a graphics pipeline
	class Pipeline {

	public:
		Pipeline() = default;
		virtual ~Pipeline() = default;

		virtual VkPipeline getPipeline() const = 0;
		virtual VkPipelineLayout getPipelineLayout() const = 0;
	};
}
