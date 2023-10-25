#pragma once

#include <GLAD/glad.h>
#include <iostream>
#include <signal.h>

#ifdef DEBUG

#ifdef _MSC_VER
#define BREAK() __debugbreak();
#elif __clang__
#define BREAK() raise(SIGTRAP);
#elif __GNUC__
#define BREAK() __asm__("BKPT 0");
#endif

namespace kge::debug
{
inline void gl_clear_error()
{
	while (glGetError() != GL_NO_ERROR)
		;
}
inline bool gl_check_error()
{
	while (auto error = glGetError())
	{
		std::cerr << "[OpenGL] (ERROR): " << error << '\n';
		return false;
	}
	return true;
}
} // namespace kge::debug

#define ASSERT(x)                                                                                                      \
	if (!(x))                                                                                                          \
	BREAK()

#define GL_CALL(x)                                                                                                     \
	kge::debug::gl_clear_error();                                                                                      \
	x;                                                                                                                 \
	ASSERT(kge::debug::gl_check_error());

#else
#define BREAK()
#define ASSERT(x)
#define GL_CALL(x) x;
#endif
