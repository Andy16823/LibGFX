// LibGFX.cpp: Definiert den Einstiegspunkt für die Anwendung.
//
#include "LibGFX.h"

using namespace std;
using namespace LibGFX;

GLFWwindow* GFX::CreateContext(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	return window;
}