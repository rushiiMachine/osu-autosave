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
add_executable(osu_autosave
        main.cpp
        memory.cpp
        osu.cpp
        process.cpp
)
target_link_libraries(osu_autosave PRIVATE ctfp::ctfp)
target_compile_features(osu_autosave PUBLIC cxx_std_23)
target_link_options(osu_autosave PUBLIC
        -m32
)
target_compile_options(osu_autosave PUBLIC
        -m32
        $<$<CONFIG:Debug>: -Wall -Werror>
        $<$<CONFIG:Release>: -O3>
)
