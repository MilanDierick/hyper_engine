cmake_minimum_required(VERSION 3.22.3)
project(hyper)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
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
            src/hyper/core/timer.h
            src/hyper/core/arena_allocator.cpp src/hyper/core/arena_allocator.h)

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
                      glfw
                      glm::glm
                      nlohmann_json::nlohmann_json
                      spdlog::spdlog
                      Vulkan::Vulkan
                      )

target_link_libraries(hyper PUBLIC
                      fmt::fmt
                      )

target_compile_options(hyper PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
                       /wd4251
                       )

target_compile_definitions(hyper PRIVATE
                           HP_DEBUG
                           DLL_EXPORT
                           GLFW_INCLUDE_NONE
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