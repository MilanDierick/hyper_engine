cmake_minimum_required(VERSION 3.23)
project(hyper C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE) # This is needed for FMT to successfully link

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "Build static or shared library using precompiled header to speed up compilation time" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library instead of bundled" FORCE)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")

add_subdirectory(thirdparty/fmt)
add_subdirectory(thirdparty/json)
add_subdirectory(thirdparty/spdlog)

add_library(hyper SHARED
            include/hyper.h
            src/base/application.cpp
            src/base/application.h
            src/base/config.h
            src/base/entrypoint.h
            src/base/log.cpp
            src/base/log.h
            src/base/parser.cpp
            src/base/parser.h
            src/base/settings.cpp
            src/base/settings.h
            src/base/asserts.h
            src/base/base.h
            src/base/window.cpp
            src/base/window.h
            )

target_precompile_headers(hyper PRIVATE
                          src/hppch.cpp
                          src/hppch.h
                          )

target_include_directories(hyper PUBLIC
                           include
                           src
                           src/base
                           thirdparty/fmt/include
                           thirdparty/json/include
                           thirdparty/spdlog/include
                           )

target_link_libraries(hyper PRIVATE
                      fmt::fmt
                      nlohmann_json::nlohmann_json
                      spdlog::spdlog
                      )

target_compile_options(hyper PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
                       )

target_compile_definitions(hyper PRIVATE
                           HP_DEBUG
                           DLL_EXPORT
                           )

add_custom_command(
		TARGET hyper POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_SOURCE_DIR}/hyper/assets/settings.json
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/settings.json
)