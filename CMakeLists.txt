cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 17)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()


include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(REQUIRES gtest/1.10.0
        BASIC_SETUP
        SETTINGS compiler.libcxx=libstdc++11)
conan_cmake_run(REQUIRES benchmark/1.5.2
        BASIC_SETUP
        SETTINGS compiler.libcxx=libstdc++11)

enable_testing()
file(GLOB auto_sources day_*/**.cpp)
add_executable(tests ${auto_sources})
target_link_libraries(tests gtest_main ${CONAN_LIBS})
target_compile_options(tests PRIVATE -Werror -Wall -Wextra)
add_executable(benchmarks ${auto_sources} benchmark.cpp)
target_link_libraries(benchmarks ${CONAN_LIBS})

FILE(GLOB resource_files resources/**.txt)
FILE(COPY ${resource_files} DESTINATION "${CMAKE_BINARY_DIR}")

include(GoogleTest)
gtest_discover_tests(tests)
