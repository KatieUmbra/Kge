module;

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

// Standard library
#include <functional>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <unordered_map>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#define NDEBUG

export module kge;

typedef void (*glfw_callback)(GLFWwindow*, int, int, int, int);
#define GLFW_CB_PARAMS GLFWwindow *wnd, int key, int scancode, int action, int mods

void gl_viewport_update(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

export namespace kge
{
struct key_input
{
	int key;
	int scancode;
	int action;
	int mods;

	bool operator==(const key_input& other) const
	{
		return key == other.key && scancode == other.scancode && action == other.action && mods == other.mods;
	}
};
} // namespace kge

namespace std
{
template <> struct hash<kge::key_input>
{
	size_t operator()(kge::key_input s) const noexcept
	{
		size_t data = 0;
		data |= s.key;
		data <<= sizeof(int);
		data |= s.scancode;
		data <<= sizeof(int);
		data |= s.action;
		data <<= sizeof(int);
		data |= s.mods;
		return data;
	}
};
} // namespace std

export namespace kge
{
[[maybe_unused]] auto library = glfwInit();
struct window_hint
{
	int hint;
	int value;
};
void add_hints(std::initializer_list<window_hint>&& hints)
{
	for (auto&& hint : hints)
	{
		glfwWindowHint(hint.hint, hint.value);
	}
}

class window
{
	GLFWwindow* m_window;
	unsigned int m_width, m_height;
	int m_xpos, m_ypos;
	bool m_fullscreen;
	int m_old_width, m_old_height, m_old_xpos, m_old_ypos;
	std::unordered_map<key_input, std::function<void()>> m_key_callbacks;
	GLFWmonitor* m_monitor;

public:
	window(unsigned int width, unsigned int height, const char* title, const char* icon = nullptr)
		: m_width{width}, m_height{height}, m_window{glfwCreateWindow(width, height, title, nullptr, nullptr)},
		  m_fullscreen{false}, m_monitor{glfwGetPrimaryMonitor()}
	{
		if (this->m_window == nullptr)
		{
			std::cerr << "Failed To Initialize GLFW Window" << '\n';
			glfwTerminate();
			std::terminate();
		}

		glfwMakeContextCurrent(this->m_window);

		if (icon != nullptr)
		{
			GLFWimage image;
			image.pixels = stbi_load(icon, &image.width, &image.height, 0, 4);
			glfwSetWindowIcon(this->m_window, 1, &image);
			stbi_image_free(image.pixels);
		}

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cerr << "Failed To Initialize GLAD" << '\n';
			glfwTerminate();
			std::terminate();
		}

		glViewport(0, 0, this->m_width, this->m_height);
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
		glfwSetFramebufferSizeCallback(this->m_window, gl_viewport_update);
		glfwSetWindowUserPointer(this->m_window, this);
		glfwSetKeyCallback(this->m_window, [](GLFW_CB_PARAMS) -> void {
			auto* it = static_cast<window*>(glfwGetWindowUserPointer(wnd));
			if (it == nullptr)
				return;
			key_input keys = {key, scancode, action, mods};
			auto callback = it->get_key_callbacks(keys);
			if (callback.has_value())
			{
				(*callback.value())();
			}
		});
		this->set_key_callback({GLFW_KEY_F11, glfwGetKeyScancode(GLFW_KEY_F11), GLFW_PRESS, 0},
							   [this]() { this->change_fullscreen(); });
	}

	~window()
	{
		glfwSetWindowUserPointer(this->m_window, nullptr);
		glfwDestroyWindow(this->m_window);
		glfwTerminate();
	}

	inline bool loop() const
	{
		return !glfwWindowShouldClose(this->m_window);
	}

	inline void updateBuffers()
	{
		glfwSwapBuffers(this->m_window);
		glfwPollEvents();
	}

	inline void clear()
	{
		GLFWmonitor* mode;
		int width, height;
		int xpos, ypos;
		glfwGetWindowSize(this->m_window, &width, &height);
		glfwGetWindowPos(this->m_window, &xpos, &ypos);
		this->m_width = width;
		this->m_height = height;
		this->m_xpos = xpos;
		this->m_ypos = ypos;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	inline void clear(glm::vec4&& color)
	{
		this->clear();
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void resize(glm::vec2 size)
	{
		glfwSetWindowSize(this->m_window, size.x, size.y);
	}

	inline int get_width() const
	{
		return this->m_width;
	}

	inline int get_height() const
	{
		return this->m_height;
	}

	inline const GLFWwindow* get_glfw_window() const
	{
		return this->m_window;
	}

	void change_fullscreen()
	{
		if (this->m_fullscreen)
		{
			this->m_fullscreen = false;
			glfwSetWindowMonitor(this->m_window, nullptr, this->m_old_xpos, this->m_old_ypos, this->m_old_width,
								 this->m_old_height, 0);
			glfwSetWindowPos(this->m_window, this->m_old_xpos, this->m_old_ypos);
			this->m_width = this->m_old_width;
			this->m_height = this->m_old_height;
			this->m_xpos = this->m_old_xpos;
			this->m_ypos = this->m_old_ypos;
			return;
		}
		this->m_fullscreen = true;
		this->m_old_width = this->m_width;
		this->m_old_height = this->m_height;
		this->m_old_xpos = this->m_xpos;
		this->m_old_ypos = this->m_ypos;
		const auto* mode = glfwGetVideoMode(this->m_monitor);
		glfwSetWindowMonitor(this->m_window, this->m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}

	void set_key_callback(key_input&& key, std::function<void()>&& callback)
	{
		this->m_key_callbacks.insert_or_assign(key, callback);
	}

	std::optional<std::function<void()>*> get_key_callbacks(const key_input& key)
	{
		if (this->m_key_callbacks.find(key) != this->m_key_callbacks.end())
		{
			return std::optional<std::function<void()>*>{&this->m_key_callbacks[key]};
		}
		return std::nullopt;
	}
};
} // namespace kge
