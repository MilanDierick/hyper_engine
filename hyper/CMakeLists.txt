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

add_subdirectory(thirdparty/fmt)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/json)
add_subdirectory(thirdparty/shaderc)
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/spirv-cross)
add_subdirectory(thirdparty/stb)

set(HYPER_SOURCES_PLATFORM_OPENGL
    src/platform/opengl/OpenGLBuffer.cpp
    src/platform/opengl/OpenGLBuffer.h
    src/platform/opengl/opengl_context.cpp
    src/platform/opengl/opengl_context.h
    src/platform/opengl/OpenGLFramebuffer.cpp
    src/platform/opengl/OpenGLFramebuffer.h
    src/platform/opengl/OpenGLRendererAPI.cpp
    src/platform/opengl/OpenGLRendererAPI.h
    src/platform/opengl/OpenGLShader.cpp
    src/platform/opengl/OpenGLShader.h
    src/platform/opengl/OpenGLTexture.cpp
    src/platform/opengl/OpenGLTexture.h
    src/platform/opengl/OpenGLUniformBuffer.cpp
    src/platform/opengl/OpenGLUniformBuffer.h
    src/platform/opengl/opengl_vertex_array.cpp
    src/platform/opengl/opengl_vertex_array.h
    )

set(HYPER_SOURCES_RENDERER
    src/hyper/renderer/buffer.cpp
    src/hyper/renderer/buffer.h
    src/hyper/renderer/camera.h
    src/hyper/renderer/editor_camera.cpp
    src/hyper/renderer/editor_camera.h
    src/hyper/renderer/framebuffer.cpp
    src/hyper/renderer/framebuffer.h
    src/hyper/renderer/graphics_context.cpp
    src/hyper/renderer/graphics_context.h
    src/hyper/renderer/orthographic_camera.cpp
    src/hyper/renderer/orthographic_camera.h
    src/hyper/renderer/orthographic_camera_controller.cpp
    src/hyper/renderer/orthographic_camera_controller.h
    src/hyper/renderer/render_command.cpp
    src/hyper/renderer/render_command.h
    src/hyper/renderer/renderer.cpp
    src/hyper/renderer/renderer.h
    src/hyper/renderer/Renderer2D.cpp
    src/hyper/renderer/Renderer2D.h
    src/hyper/renderer/renderer_api.cpp
    src/hyper/renderer/renderer_api.h
    src/hyper/renderer/Shader.cpp
    src/hyper/renderer/Shader.h
    src/hyper/renderer/Texture.cpp
    src/hyper/renderer/Texture.h
    src/hyper/renderer/UniformBuffer.cpp
    src/hyper/renderer/UniformBuffer.h
    src/hyper/renderer/vertex_array.cpp
    src/hyper/renderer/vertex_array.h
    )

add_library(hyper SHARED
            ${HYPER_SOURCES_PLATFORM_OPENGL}
            ${HYPER_SOURCES_RENDERER}
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
            src/platform/universal/universal_window.cpp
            src/platform/universal/universal_window.h
            src/hyper/events/mouse_event_args.h
            src/hyper/core/mouse_codes.h
            src/hyper/core/input.h
            src/platform/universal/universal_input.cpp
            src/hyper/core/timer.h)

target_precompile_headers(hyper PRIVATE
                          src/hppch.cpp
                          src/hppch.h
                          )

target_include_directories(hyper PUBLIC
                           include
                           src
                           src/hyper/core
                           thirdparty
                           thirdparty/fmt/include
                           thirdparty/glad/include
                           thirdparty/glfw/include
                           thirdparty/glm/
                           thirdparty/json/include
                           thirdparty/shaderc/glslc/src
                           thirdparty/shaderc/libshaderc/include
                           thirdparty/shaderc/libshaderc_util/include
                           thirdparty/spdlog/include
                           thirdparty/spirv-cross/include
                           thirdparty/spirv-tools/include
                           thirdparty/stb
                           )

target_link_libraries(hyper PRIVATE
                      fmt::fmt
                      glad
                      glfw
                      glm::glm
                      nlohmann_json::nlohmann_json
                      spdlog::spdlog
                      shaderc
                      spirv-cross-core
                      spirv-cross-glsl
                      stb
                      )

target_compile_options(hyper PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
                       )

target_compile_definitions(hyper PRIVATE
                           HP_DEBUG
                           DLL_EXPORT
                           GLFW_INCLUDE_NONE
                           )

add_custom_command(
		TARGET hyper POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/hyper/assets/settings.json
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/settings.json
)