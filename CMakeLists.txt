cmake_minimum_required(VERSION 3.28)
project(OGL2DGame LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_SOURCE_DIR}/include -isystem /usr/local/include/")
add_compile_definitions(SOURCE_DIRECTORY="${CMAKE_SOURCE_DIR}")

find_library(glfw
	NAMES libglfw libglfw3 glfw glfw3
	PATHS /usr/local/lib
)
find_library(glad
	NAMES libglad glad
	PATHS /usr/local/lib
)

set(SOURCE src/main.cpp)
set(SOURCES src/window.cpp)
add_library(engine STATIC)
target_sources(engine PUBLIC FILE_SET CXX_MODULES FILES 
	${SOURCES}
)
target_compile_features(engine PUBLIC cxx_std_20)
target_include_directories(engine PUBLIC /usr/local/include ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(engine PUBLIC ${glfw} ${glad})

add_executable(oglgame ${SOURCE})
target_include_directories(oglgame PUBLIC /usr/local/include ${CMAKE_SOURCE_DIR}/include)
target_compile_features(oglgame PUBLIC cxx_std_20)
target_link_libraries(oglgame PUBLIC engine)

