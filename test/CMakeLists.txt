find_package(doctest REQUIRED)
include(CTest)

set(T1 CanMessageTest)
add_executable(${T1} CanMessage.cpp)
target_include_directories(${T1} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${T1} PRIVATE ${PROJECT_NAME} doctest::doctest)
add_test(NAME ${T1} COMMAND ${T1})

set(T2 slcanInterfaceTest)
add_executable(${T2} slcanInterface.cpp)
target_include_directories(${T2} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${T2} PRIVATE ${PROJECT_NAME} doctest::doctest)
add_test(NAME ${T2} COMMAND ${T2})
