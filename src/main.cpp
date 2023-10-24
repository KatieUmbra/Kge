import kge;

#include <GLFW/glfw3.h>
#include <iostream>

auto main() -> int
{
	kge::add_hints({{GLFW_CONTEXT_VERSION_MAJOR, 4},
					{GLFW_CONTEXT_VERSION_MINOR, 6},
					{GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE},
					{GLFW_FLOATING, GL_TRUE},
					{GLFW_DECORATED, GL_FALSE}});
#ifndef _WIN32
	glfwWindowHintString(GLFW_X11_CLASS_NAME, "ogl-game");
	glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "ogl-game");
#endif

	const char* icon_path = SOURCE_DIRECTORY R"(/assets/icon.png)";

	kge::window window{800, 800, "Hello OpenGL", icon_path};

	std::hash<kge::key_input> data;
	std::cout << data({1, 2, 3, 4}) << '\n';

	while (window.loop())
	{
		window.clear({1.0f, 0.8f, 0.8f, 1.0f});
		window.updateBuffers();
	}
}
