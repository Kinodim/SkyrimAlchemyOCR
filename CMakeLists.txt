cmake_minimum_required(VERSION 2.8)
project(SHOW_IMAGE)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
add_executable(alchemy-ocr main.cpp)
add_executable(recipe_generator recipe_generator.cpp)
target_link_libraries(alchemy-ocr ${OpenCV_LIBS} tesseract)
