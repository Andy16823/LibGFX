#pragma once
#include <vulkan/vulkan.h>
#include "RenderPass.h"


namespace LibGFX {
	namespace Presets {
		class DefaultRenderPass : public LibGFX::RenderPass
		{
		private:
			VkRenderPass m_renderPass;
		public:
			VkRenderPass getRenderPass() const override;
			bool create(VkDevice device, VkFormat swapchainImageFormat, VkFormat depthFormat) override;
			void destroy(VkDevice device) override;
			std::span<const VkClearValue> getClearValues() const override;
		};
	}
}
