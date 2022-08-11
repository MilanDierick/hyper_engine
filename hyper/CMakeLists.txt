cmake_minimum_required(VERSION 3.23)
project(hyper C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE) # This is needed for FMT to successfully link

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" FORCE)
set(SPDLOG_ENABLE_PCH ON CACHE BOOL "Build static or shared library using precompiled header to speed up compilation time" FORCE)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "Use external fmt library instead of bundled" FORCE)

add_subdirectory(thirdparty/fmt)
add_subdirectory(thirdparty/spdlog)

add_library(hyper SHARED src/base/entrypoint.h src/base/application.cpp src/base/application.h include/hyper.h src/base/log.cpp src/base/log.h)

target_precompile_headers(hyper PRIVATE src/hppch.cpp src/hppch.h)

target_include_directories(hyper PUBLIC
                           include
                           src
                           src/base
                           thirdparty/fmt/include
                           thirdparty/spdlog/include
                           )

target_link_libraries(hyper PRIVATE
                      spdlog::spdlog
                      )

target_link_libraries(hyper PUBLIC
                      fmt::fmt # Any client library also needs to link to fmt for some reason
                      )

target_compile_options(hyper PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                       $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
                       )

target_compile_definitions(hyper PRIVATE
                           HL_DEBUG
                           BUILD_SHARED_LIBS
                           )