cmake_minimum_required(VERSION 3.10)

project(lab-2)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp    
    src/image.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)