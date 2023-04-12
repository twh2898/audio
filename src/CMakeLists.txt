set(TARGET app)

set(HEADER_LIST
    plugins/delay.hpp
    plugins/gain.hpp
    plugins/oscillator.hpp
    plugins/plugin.hpp
    bus.hpp
    context.hpp
    sound.hpp
    util.hpp
)
list(TRANSFORM HEADER_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/include/")

set(SOURCE_LIST main.cpp)
list(TRANSFORM SOURCE_LIST PREPEND "${${PROJECT_NAME}_SOURCE_DIR}/src/")

add_executable(${TARGET} ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(${TARGET} PUBLIC
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${TARGET} PRIVATE cxx_std_17)
target_link_libraries(${TARGET} PRIVATE OpenAL::OpenAL)
