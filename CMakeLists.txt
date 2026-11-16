cmake_minimum_required(VERSION 3.31)

project(SO-Projekt
	VERSION 0.1
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)


include(FetchContent)
FetchContent_Declare(
	Corrosion
	GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
	GIT_TAG v0.5.0
)
FetchContent_MakeAvailable(Corrosion)

add_subdirectory(src)
