cmake_minimum_required(VERSION 3.22)
project(samplelibrary-project VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP_SUPPORT_FLEXIBLE_PAGE_SIZES ON)

add_subdirectory(modules/samplelibrary)
