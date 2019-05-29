# 2019-05-25 by Manlou Hua(hua7th@gmail.com)
#
cmake_minimum_required (VERSION 2.8)
project (tcp-server)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
aux_source_directory (. DIR_SRCS)
add_executable (server ${DIR_SRCS})