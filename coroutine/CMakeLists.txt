# 2019-05-29 by Manlou Hua(hua7th@gmail.com)
#
cmake_minimum_required (VERSION 2.8)
project (simple-fiber)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS context fiber)

if(Boost_FOUND)
    aux_source_directory (. DIR_SRCS)
    add_executable (simple ${DIR_SRCS})
    target_link_libraries(simple Boost::fiber Boost::context)
else()
    message(err: Boost fiber not found)
endif()



