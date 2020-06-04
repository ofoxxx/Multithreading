cmake_minimum_required (VERSION 3.8)

project ("DynamicPool" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(DynamicPool STATIC "DynamicPool.cpp" "DynamicPool.h" "CASQueue.h")
