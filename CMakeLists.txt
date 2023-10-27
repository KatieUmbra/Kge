cmake_minimum_required(VERSION 3.28)
project(OGL2DGame LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(SOURCE_DIRECTORY="${CMAKE_SOURCE_DIR}")

IF(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${CMAKE_SOURCE_DIR}/libs/include ")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:library /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

find_library(glfw NAMES glfw glfw3 libglfw libglfw3 PATHS ${CMAKE_SOURCE_DIR}/libs/binaries)
find_library(glad NAMES glad libglad PATHS ${CMAKE_SOURCE_DIR}/libs/binaries)
ELSE()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -isystem ${CMAKE_SOURCE_DIR}libs/include")
set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad)
find_library(glfw NAMES glfw glfw3 libglfw libglfw3)
find_library(glad NAMES glad libglad)
ENDIF()

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

set(SOURCE src/main.cpp)
set(SOURCES_KGE 
	src/kge.c++
	src/kge_window.cpp 
	src/kge_logging.cpp

	src/kge_renderer.c++ 
	src/kge_renderer_vertex_buffer.cpp
	src/kge_renderer_vertex_array.cpp
	src/kge_renderer_index_buffer.cpp
	src/kge_renderer_material.cpp
	src/kge_renderer_shader.cpp
	src/kge_renderer_texture.cpp
	)

add_library(kge STATIC)
target_sources(kge PUBLIC FILE_SET CXX_MODULES FILES 
	${SOURCES_KGE}
)

target_compile_features(kge PUBLIC cxx_std_20)
target_link_libraries(kge PUBLIC ${glfw} ${glad})

add_executable(oglgame ${SOURCE})
target_compile_features(oglgame PUBLIC cxx_std_20)
target_link_libraries(oglgame PUBLIC kge)

target_include_directories(kge PUBLIC ${CMAKE_SOURCE_DIR}/libs/include)
target_include_directories(oglgame PUBLIC ${CMAKE_SOURCE_DIR}/libs/include)
