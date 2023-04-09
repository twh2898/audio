cmake_minimum_required(VERSION 3.15...3.20)
project(project VERSION 0.0.0 LANGUAGES CXX)

#find_package(fmt REQUIRED)

find_package(OpenAL REQUIRED)

set(TARGET app)
add_executable(${TARGET} main.cpp sound.hpp)
target_compile_features(${TARGET} PRIVATE cxx_std_17)
target_link_libraries(${TARGET} PRIVATE OpenAL::OpenAL)

#target_link_libraries(${TARGET} PRIVATE fmt::fmt)

