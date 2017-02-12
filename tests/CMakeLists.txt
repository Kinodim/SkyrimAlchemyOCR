SET(SRC_DIR ${ALCHEMY_OCR_SOURCE_DIR}/src)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${ALCHEMY_OCR_SOURCE_DIR}/include)

add_executable(Translator_tests Translator_tests.cpp ${SRC_DIR}/Translator.cpp)
target_link_libraries(Translator_tests ${GTEST_LIBRARIES} pthread)

add_executable(OCRProcessor_tests OCRProcessor_tests.cpp ${SRC_DIR}/OCRProcessor.cpp 
	${SRC_DIR}/Translator.cpp ${SRC_DIR}/levenshtein.cpp)
target_link_libraries(OCRProcessor_tests ${GTEST_LIBRARIES} pthread)

add_executable(Recipe_tests Recipe_tests.cpp)
target_link_libraries(Recipe_tests ${GTEST_LIBRARIES} pthread)

add_executable(Inventory_tests Inventory_tests.cpp)
target_link_libraries(Inventory_tests ${GTEST_LIBRARIES} pthread)

#add_executable(Recipes_tests Recipes_tests.cpp)
