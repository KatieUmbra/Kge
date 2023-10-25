module;

#include "assert.hpp"
#include <GLAD/glad.h>
#include <array>

export module kge_renderer:vertex_buffer;

import :vertex_array;

export namespace kge
{
class vertex_buffer
{
	unsigned int m_buffer_id;
	GLenum m_buffer_type;

public:
	template <typename T, size_t M> vertex_buffer(const std::array<T, M>& data, GLenum type) : m_buffer_type(type)
	{
		GL_CALL(glGenBuffers(1, &m_buffer_id));
		this->bind();
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data, type));
	}

	~vertex_buffer()
	{
		this->unbind();
		GL_CALL(glDeleteBuffers(1, &this->m_buffer_id));
	}

	void bind() const
	{
		GL_CALL(glBindBuffer(this->m_buffer_type, this->m_buffer_id));
	}

	void unbind() const
	{
		GL_CALL(glBindBuffer(this->m_buffer_type, 0));
	}

	template <typename T, size_t M> void set_data(const std::array<T, M>& data)
	{
		this->bind();
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data, this->m_buffer_type));
	}
};
} // namespace kge
