module;

#include "assert.hpp"
#include <GLAD/glad.h>
#include <array>

export module renderer:vertex_buffer;

import :vertex_array;

export namespace kge
{
template <size_t N> class vertex_buffer
{
	std::array<int, N> m_buffer_ids;
	GLenum m_buffer_type;

public:
	template <typename T, size_t M> vertex_buffer(const std::array<T, M>& data, GLenum type) : m_buffer_type(type)
	{
		GL_CALL(glGenBuffers(N, &m_buffer_ids));
		this->bind();
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), &data, type));
	}

	~vertex_buffer()
	{
		GL_CALL(glDeleteBuffers(N, &this->m_buffer_ids));
	}

	void bind(unsigned int index) const
	{
		if (index >= this->m_buffer_ids.size())
		{
			std::cerr << "[Kge] (ERROR): Buffer id overflow" << '\n';
			return;
		}
		GL_CALL(glBindBuffer(this->m_buffer_type, this->m_buffer_ids[index]));
	}

	void unbind() const
	{
		GL_CALL(glBindBuffer(this->m_buffer_type, nullptr));
	}

	template <typename T, size_t M> void set_data(const std::array<T, M>& data)
	{
		this->bind();
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), &data, this->m_buffer_type));
	}
};
} // namespace kge
