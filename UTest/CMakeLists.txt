cmake_minimum_required (VERSION 3.8)

project ("UTest" LANGUAGES CXX)

include_directories("../Spinlock")

add_library(UTest STATIC "UTestCase.cpp" "UTestCase.h")
