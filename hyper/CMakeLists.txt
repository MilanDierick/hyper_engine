cmake_minimum_required(VERSION 3.22.3)
project(hyper)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(thirdparty/vk-bootstrap)
add_subdirectory(thirdparty/VulkanMemoryAllocator)

add_library(glfw::glfw ALIAS glfw)

file(GLOB_RECURSE CPPSANDBOX_SOURCE_FILES
        "src/hyper/*.h"
        "src/hyper/*.cpp"
        "src/platform/*.h"
        "src/platform/*.cpp"
        )

add_library(hyper SHARED ${CPPSANDBOX_SOURCE_FILES})

target_precompile_headers(hyper PRIVATE
        src/hppch.cpp
        src/hppch.h
        )

target_include_directories(hyper PUBLIC
        include
        src
        thirdparty/vk-bootstrap/src
        thirdparty/VulkanMemoryAllocator/include
        )

target_compile_options(hyper PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4251 /wd4100>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror -Wno-nullability-extension>
        )

target_compile_definitions(hyper PRIVATE
        HP_DEBUG
        DLL_EXPORT
        GLFW_INCLUDE_VULKAN
        )

target_link_libraries(hyper PUBLIC
        glfw::glfw
        glm::glm
        spdlog::spdlog
        Vulkan::Vulkan
        VulkanMemoryAllocator
        vk-bootstrap
        )

add_custom_command(
        TARGET hyper POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/hyper/assets/settings.json
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/settings.json
)