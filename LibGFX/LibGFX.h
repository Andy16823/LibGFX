#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

GLFWwindow* CreateContext(int width, int height, const char* title);