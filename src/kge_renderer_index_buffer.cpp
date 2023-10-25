module;

#include "assert.hpp"
#include <GLAD/glad.h>
#include <array>

export module kge_renderer:index_buffer;

export namespace kge
{
class index_buffer
{
public:
	unsigned int m_buffer_id;
	GLenum m_type;
	template <size_t N> index_buffer(const std::array<unsigned int, N>& data, GLenum type) : m_type{type}
	{
		GL_CALL(glGenBuffers(1, &this->m_buffer_id));
		this->bind();
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data, this->m_type));
	}

	~index_buffer()
	{
		this->unbind();
		GL_CALL(glDeleteBuffers(1, &this->m_buffer_id));
	}

	void bind()
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer_id));
	}

	void unbind()
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	auto get_buffer_id() -> unsigned int
	{
		return this->m_buffer_id;
	}

	template <size_t N> void set_data(const std::array<unsigned int, N>& data, GLenum type)
	{
		this->m_type = type;
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * data.size(), &data, this->m_type));
	}
};
} // namespace kge
