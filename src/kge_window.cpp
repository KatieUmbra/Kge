module;

// External Deps
#define STB_IMAGE_IMPLEMENTATION
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <STB/stb_image.h>

// Standard library
#include <functional>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <unordered_map>

#define NDEBUG

export module kge:window;

namespace kge
{
typedef void (*glfw_callback)(GLFWwindow*, int, int, int, int);
#define GLFW_CB_PARAMS GLFWwindow *wnd, int key, int scancode, int action, int mods

inline void gl_viewport_update(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
} // namespace kge

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
	union {
		int id;
		const char* value;
	};
};
void add_hints(std::initializer_list<window_hint>&& hints)
{
	for (auto&& hint : hints)
	{
		glfwWindowHint(hint.hint, hint.id);
	}
}

void add_string_hints(std::initializer_list<window_hint>&& hints)
{
	for (auto&& hint : hints)
	{
		glfwWindowHintString(hint.hint, hint.value);
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

	inline void glfw_position_update(int xpos, int ypos)
	{
		this->m_xpos = xpos;
		this->m_ypos = ypos;
	}

	inline void glfw_size_update(int width, int height)
	{
		this->m_width = width;
		this->m_height = height;
	}

	inline void set_callbacks()
	{
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

		glfwSetWindowPosCallback(this->m_window, [](GLFWwindow* window, int xpos, int ypos) -> void {
			static_cast<class window*>(glfwGetWindowUserPointer(window))->glfw_position_update(xpos, ypos);
		});

		glfwSetWindowSizeCallback(this->m_window, [](GLFWwindow* window, int width, int height) -> void {
			static_cast<class window*>(glfwGetWindowUserPointer(window))->glfw_size_update(width, height);
		});
	}

	void restore_old_values()
	{
		this->m_width = this->m_old_width;
		this->m_height = this->m_old_height;
		this->m_xpos = this->m_old_xpos;
		this->m_ypos = this->m_old_ypos;
	}

	void set_old_values()
	{
		this->m_old_width = this->m_width;
		this->m_old_height = this->m_height;
		this->m_old_xpos = this->m_xpos;
		this->m_old_ypos = this->m_ypos;
	}

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
			this->set_icon(icon);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		{
			std::cerr << "Failed To Initialize GLAD" << '\n';
			glfwTerminate();
			std::terminate();
		}

		this->set_callbacks();

		this->set_key_callback({GLFW_KEY_F11, glfwGetKeyScancode(GLFW_KEY_F11), GLFW_PRESS, 0},
							   [this]() { this->change_fullscreen(); });

		glViewport(0, 0, this->m_width, this->m_height);

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
	}

	~window()
	{
		glfwSetWindowUserPointer(this->m_window, nullptr);
		glfwDestroyWindow(this->m_window);
		glfwSetWindowSizeCallback(nullptr, nullptr);
		glfwSetWindowPosCallback(nullptr, nullptr);
		glfwSetKeyCallback(nullptr, nullptr);
		glfwSetFramebufferSizeCallback(nullptr, nullptr);
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
		glViewport(0, 0, size.x, size.y);
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
			this->restore_old_values();
			return;
		}
		this->m_fullscreen = true;
		this->set_old_values();
		const auto* mode = glfwGetVideoMode(this->m_monitor);
		glfwSetWindowMonitor(this->m_window, this->m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}

	void set_icon(const char* icon)
	{
		GLFWimage image;
		image.pixels = stbi_load(icon, &image.width, &image.height, 0, 4);
		glfwSetWindowIcon(this->m_window, 1, &image);
		stbi_image_free(image.pixels);
	}

	inline void set_key_callback(key_input&& key, std::function<void()>&& callback)
	{
		this->m_key_callbacks.insert_or_assign(key, callback);
	}

	auto get_key_callbacks(const key_input& key) -> std::optional<std::function<void()>*>
	{
		if (this->m_key_callbacks.find(key) != this->m_key_callbacks.end())
		{
			return std::optional<std::function<void()>*>{&this->m_key_callbacks[key]};
		}
		return std::nullopt;
	}
};
} // namespace kge
