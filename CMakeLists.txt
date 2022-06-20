cmake_minimum_required(VERSION 3.22)
project(Learning_GoogleMock)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(MyLibrary)
add_subdirectory(tests)

# Get googletest
include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
#googletest_POPULATED
#googletest_SOURCE_DIR
#googletest_BUILD_DIR
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

add_executable(Learning_GoogleMock main.cpp)

target_link_libraries(${PROJECT_NAME}
        MyLibrary
        gtest
        gtest_main
        gmock_main
        )