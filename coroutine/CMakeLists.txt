# 2019-07-12 by Web Surfing(qingdujun@outlook.com)
#
cmake_minimum_required (VERSION 2.8)
project (coroutine-example)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS context coroutine)

if(Boost_FOUND)
    add_executable (example coroutine2-example.cpp)
    target_link_libraries(example Boost::coroutine Boost::context)
else()
    message(err: Boost coroutine not found)
endif()