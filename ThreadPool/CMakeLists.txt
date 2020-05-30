cmake_minimum_required (VERSION 3.8)

project ("ThreadPool" LANGUAGES CXX)

add_library(ThreadPool STATIC "ThreadPool.cpp" "ThreadPool.h")
