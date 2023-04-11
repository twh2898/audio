cmake_minimum_required(VERSION 3.15...3.20)
project(project VERSION 0.0.0 LANGUAGES CXX)

# find_package(fmt REQUIRED)
find_package(OpenAL REQUIRED)

add_subdirectory(src)
