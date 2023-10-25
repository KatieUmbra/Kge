module;

#include "assert.hpp"
#include <GLAD/glad.h>

export module kge_renderer:vertex_array;

export namespace kge
{
class vertex_array
{
	unsigned int m_array_id;

public:
	vertex_array()
	{
		GL_CALL(glGenVertexArrays(1, &this->m_array_id));
	}

	~vertex_array()
	{
		GL_CALL(glDeleteVertexArrays(1, &this->m_array_id));
	}

	void bind() const
	{
		GL_CALL(glBindVertexArray(this->m_array_id));
	}

	void unbind() const
	{
		GL_CALL(glBindVertexArray(0));
	}

	void add_buffer()
	{
	}
};
} // namespace kge
