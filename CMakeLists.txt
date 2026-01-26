cmake_minimum_required(VERSION 3.20)
project(
  ap3
  VERSION 0.0.0
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
# find_package(PkgConfig REQUIRED)

add_compile_options(
  -Wall -Wextra -Wpedantic -Wshadow
  # -Wconversion -Werror
)

add_library(${PROJECT_NAME} STATIC src/CanMessage.cpp src/slcanInterface.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(can_app src/main.cpp)
target_link_libraries(can_app PRIVATE ${PROJECT_NAME})
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

enable_testing()
add_subdirectory(test)
