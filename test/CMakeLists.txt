find_package(doctest REQUIRED)
include(CTest)

add_executable(CanMessage CanMessage.cpp)
target_include_directories(CanMessage PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(CanMessage PRIVATE ${PROJECT_NAME} doctest::doctest)
add_test(NAME CanMessage COMMAND CanMessage)
