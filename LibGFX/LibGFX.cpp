// LibGFX.cpp: Definiert den Einstiegspunkt für die Anwendung.
//
#include "LibGFX.h"
#include <iosfwd>
#include <fstream>

using namespace std;
using namespace LibGFX;

GLFWwindow* GFX::createWindow(int width, int height, const char* title, bool fullscreen, bool resizable)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;

	auto window = glfwCreateWindow(width, height, title, monitor, nullptr);
	return window;
}

std::vector<char> GFX::readFile(const std::string& filename)
{
	std::ifstream file;
	file.open(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + filename);
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
