#include "VulkanRenderer.h"
#include <GLFW/glfw3.h>

using namespace LibGFX;

VulkanRenderer::VulkanRenderer(GLFWwindow* targetWindow)
{
	m_targetWindow = targetWindow;
}

LibGFX::VulkanRenderer::~VulkanRenderer()
{
	m_targetWindow = nullptr;
}
