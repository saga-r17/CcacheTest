cmake_minimum_required(VERSION 3.16)

project(main)
#test1phase1
add_executable(main main.cpp)

install(TARGETS main)

enable_testing()
add_test(NAME main COMMAND main)
