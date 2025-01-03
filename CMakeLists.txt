cmake_minimum_required(VERSION 3.27)

# Dependencies

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
        ctfp
        GIT_REPOSITORY "https://github.com/pushfq/ctfp.git"
        GIT_TAG "f6b4f48"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ctfp)

# Project

project(osu_autosave CXX)
add_executable(osu_autosave main.cpp)
target_link_libraries(osu_autosave PRIVATE ctfp::ctfp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
