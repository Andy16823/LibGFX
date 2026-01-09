// LibGFX.cpp: Definiert den Einstiegspunkt für die Anwendung.
//
#include "LibGFX.h"
#include <iosfwd>
#include <fstream>

using namespace std;
using namespace LibGFX;

GLFWwindow* GFX::createContext(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	auto window = glfwCreateWindow(width, height, title, nullptr, nullptr);
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
