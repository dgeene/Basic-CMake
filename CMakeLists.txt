cmake_minimum_required (VERSION 2.6)
project (GPIOTest)
add_executable(GPIOTest1 GPIOTest1.cpp)
target_link_Libraries(GPIOTest1 GPIOClass)

# libraries
add_library(GPIOClass GPIOClass.cpp)
