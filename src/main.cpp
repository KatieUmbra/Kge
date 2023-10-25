// Standard Library
#include <iostream>

#include <GLFW/glfw3.h>

import kge;

auto main() -> int
{
#ifndef _WIN32
	kge::add_string_hints(
		{{.hint = GLFW_X11_INSTANCE_NAME, .value = "ogl-game"}, {.hint = GLFW_X11_CLASS_NAME, .value = "ogl-game"}});
#endif
	kge::add_hints({{GLFW_CONTEXT_VERSION_MAJOR, 4},
					{GLFW_CONTEXT_VERSION_MINOR, 6},
					{GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE},
					{GLFW_FLOATING, GL_TRUE},
					{GLFW_DECORATED, GL_TRUE}});

	const char* icon_path = SOURCE_DIRECTORY R"(/assets/icon.png)";

	kge::window window{800, 800, "Hello OpenGL", icon_path};

	std::hash<kge::key_input> data;
	std::cout << data({1, 2, 3, 4}) << '\n';

	while (window.loop())
	{
		window.clear({0.12f, 0.12f, 0.12f, 1.0f});
		window.updateBuffers();
	}
}
