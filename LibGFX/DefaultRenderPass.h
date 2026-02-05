#pragma once
#include <vulkan/vulkan.h>
#include "RenderPass.h"
#include "VkContext.h"


namespace LibGFX {
	namespace Presets {
		class DefaultRenderPass : public LibGFX::RenderPass
		{
		private:
			VkRenderPass m_renderPass;
		public:
			VkRenderPass getRenderPass() const override;
			bool create(VkContext& context, VkFormat swapchainImageFormat, VkFormat depthFormat) override;
			void destroy(VkContext& context) override;
			std::span<const VkClearValue> getClearValues() const override;
		};
	}
}
