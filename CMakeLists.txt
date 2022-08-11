cmake_minimum_required(VERSION 3.23)
project(playground C CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_subdirectory(hyper)

add_executable(playground
               playground/src/playground_application.cpp
               playground/src/playground_application.h
               )

target_include_directories(playground PRIVATE
                           playground/src
                           )

target_link_libraries(playground PRIVATE
                      hyper
                      )
