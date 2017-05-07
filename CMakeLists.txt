cmake_minimum_required(VERSION 3.6)
project(Code)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Movie.cpp Movie.h Documentary.cpp Memtrace.cpp Family.cpp Family.h Container.cpp Container.h Menu.cpp Menu.h)
add_executable(Code ${SOURCE_FILES})