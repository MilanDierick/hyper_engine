cmake_minimum_required(VERSION 3.23)
project(playground C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(hyper)

add_executable(playground hyper/src/base/entrypoint.h playground/src/playground_application.cpp playground/src/playground_application.h)

target_link_libraries(playground PRIVATE
                      hyper
                      spdlog::spdlog
                      fmt::fmt
                      )