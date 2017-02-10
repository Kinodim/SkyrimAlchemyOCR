SET(SRC_DIR ${ALCHEMY_OCR_SOURCE_DIR}/src)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${ALCHEMY_OCR_SOURCE_DIR}/include)
#link_directories(${ALCHEMY_OCR_SOURCE_DIR}/src)

add_executable(Translator_tests Translator_tests.cpp ${SRC_DIR}/Translator.cpp)
target_link_libraries(Translator_tests ${GTEST_LIBRARIES} pthread)
#add_executable(Recipes_tests Recipes_tests.cpp)
