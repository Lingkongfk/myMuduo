cmake_minimum_required(VERSION 3.11)
project(myMuduo)

#myMuduo最终编译成so动态库，设置动态库路径，放在根目录lib文件夹下面
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

#定义参与编译的源文件
aux_source_directory(. SRC_LIST)

#编译生成动态库
add_library(myMuduo SHARED ${SRC_LIST})
 