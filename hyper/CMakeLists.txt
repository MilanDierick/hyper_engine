cmake_minimum_required(VERSION 3.22.3)
project(hyper)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "Build static or shared library using precompiled header to speed up compilation time" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library instead of bundled" FORCE)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")

set(SHADERC_SKIP_INSTALL TRUE)
set(SHADERC_SKIP_TESTS TRUE)
set(SHADERC_SKIP_EXAMPLES TRUE)
set(SHADERC_SKIP_COPYRIGHT_CHECK TRUE)

set(SPIRV_CROSS_SHARED FALSE)
set(SPIRV_CROSS_STATIC TRUE)
set(SPIRV_CROSS_CLI FALSE)
set(SPIRV_CROSS_ENABLE_TESTS FALSE)
set(SPIRV_CROSS_ENABLE_GLSL TRUE)
set(SPIRV_CROSS_ENABLE_HLSL TRUE)
set(SPIRV_CROSS_ENABLE_MSL TRUE)
set(SPIRV_CROSS_ENABLE_CPP TRUE)
set(SPIRV_CROSS_ENABLE_REFLECT TRUE)
set(SPIRV_CROSS_ENABLE_C_API TRUE)
set(SPIRV_CROSS_ENABLE_UTIL TRUE)
set(SPIRV_CROSS_SKIP_INSTALL TRUE)

find_package(fmt REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(Vulkan REQUIRED)

set(HYPER_SOURCES_RENDERER
    
    )

set(HYPER_SOURCES_PLATFORM_VULKAN
    src/platform/vulkan/vulkan_window.cpp
    src/platform/vulkan/vulkan_window.h
    )

add_library(hyper SHARED
            ${HYPER_SOURCES_RENDERER}
            ${HYPER_SOURCES_PLATFORM_VULKAN}
            include/hyper.h
            src/hyper/core/application.cpp
            src/hyper/core/application.h
            src/hyper/core/config.h
            src/hyper/core/entrypoint.h
            src/hyper/core/log.cpp
            src/hyper/core/log.h
            src/hyper/core/parser.cpp
            src/hyper/core/parser.h
            src/hyper/core/settings.cpp
            src/hyper/core/settings.h
            src/hyper/core/asserts.h
            src/hyper/core/base.h
            src/hyper/core/window.cpp
            src/hyper/core/window.h
            src/hyper/core/layer.cpp
            src/hyper/core/layer.h
            src/hyper/core/layer_stack.cpp
            src/hyper/core/layer_stack.h
            src/hyper/events/event.h
            src/hyper/events/application_event_args.h
            src/hyper/events/key_event_args.h
            src/hyper/core/key_codes.h
            src/hyper/events/mouse_event_args.h
            src/hyper/core/mouse_codes.h
            src/hyper/core/input.h
            src/platform/universal/glfw_input.cpp
            src/hyper/core/timer.h)

target_precompile_headers(hyper PRIVATE
                          src/hppch.cpp
                          src/hppch.h
                          )

target_include_directories(hyper PUBLIC
                           include
                           src
                           src/hyper/core
                           )

target_link_libraries(hyper PRIVATE
                      fmt
                      glfw
                      glm
                      nlohmann_json
                      spdlog
                      )

target_link_libraries(hyper PUBLIC
                      fmt
                      )

target_compile_options(hyper PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
                       )

target_compile_definitions(hyper PRIVATE
                           HP_DEBUG
                           DLL_EXPORT
                           GLFW_INCLUDE_VULKAN
                           )

target_compile_definitions(hyper PUBLIC
                           SPDLOG_FMT_EXTERNAL
                           )

add_custom_command(
		TARGET hyper POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/hyper/assets/settings.json
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/settings.json
)