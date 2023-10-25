cmake_minimum_required(VERSION 3.28)
project(OGL2DGame LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_definitions(SOURCE_DIRECTORY="${CMAKE_SOURCE_DIR}")

find_library(glfw
	NAMES libglfw libglfw3 glfw glfw3
	PATHS ${CMAKE_SOURCE_DIR}/libs /usr/local/lib
)
find_library(glad
	NAMES libglad glad
	PATHS ${CMAKE_SOURCE_DIR}/libs /usr/local/lib
)

IF(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${CMAKE_SOURCE_DIR}/libs/include ")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:library /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
ELSE()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -I${CMAKE_SOURCE_DIR}/include -isystem /usr/local/include/")
set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
ENDIF()

set(SOURCE src/main.cpp)
set(SOURCES_KGE 
	src/kge.c++
	src/kge_window.cpp 
	)

set(SOURCES_RENDERER
	src/renderer.c++ 
	src/renderer_vertex_buffer.cpp
	src/renderer_vertex_array.cpp
	)

add_library(engine STATIC)
add_library(renderer STATIC)
target_sources(engine PUBLIC FILE_SET CXX_MODULES FILES 
	${SOURCES_KGE}
)

target_sources(renderer PUBLIC FILE_SET CXX_MODULES FILES
	${SOURCES_RENDERER}
	)

target_compile_features(engine PUBLIC cxx_std_20)
target_link_libraries(engine PUBLIC renderer ${glfw} ${glad})

target_compile_features(renderer PUBLIC cxx_std_20)
target_link_libraries(renderer PUBLIC engine ${glfw} ${glad})

add_executable(oglgame ${SOURCE})
target_compile_features(oglgame PUBLIC cxx_std_20)
target_link_libraries(oglgame PUBLIC engine renderer)

IF(WIN32)
target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR}/libs/include ${CMAKE_SOURCE_DIR}/include)
target_include_directories(renderer PUBLIC ${CMAKE_SOURCE_DIR}/libs/include ${CMAKE_SOURCE_DIR}/include)
target_include_directories(oglgame PUBLIC ${CMAKE_SOURCE_DIR}/libs/include ${CMAKE_SOURCE_DIR}/include)
ELSE()
target_include_directories(engine PUBLIC /usr/local/include ${CMAKE_SOURCE_DIR}/include)
target_include_directories(renderer PUBLIC /usr/local/include ${CMAKE_SOURCE_DIR}/include)
target_include_directories(oglgame PUBLIC /usr/local/include ${CMAKE_SOURCE_DIR}/include)
ENDIF()