cmake_minimum_required(VERSION 3.10)

project(lab-2)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp    
    src/image.cpp
)

add_subdirectory(libs)

find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    sfml-graphics    
    sfml-window    
    CLI11::CLI11
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)