cmake_minimum_required (VERSION 3.8)

project ("Multithreading" LANGUAGES CXX)

add_executable(main main.cpp)

add_subdirectory ("ThreadPool")
include_directories("ThreadPool")
target_link_libraries(main ThreadPool)

add_subdirectory ("UTest")
include_directories("UTest")
target_link_libraries(main UTest)
