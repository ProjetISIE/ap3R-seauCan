cmake_minimum_required(VERSION 3.20)
project(
  ap3Can
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
set(MAIN ${PROJECT_NAME}Client)
add_executable(${MAIN} src/main.cpp)
target_link_libraries(${MAIN} PRIVATE ${PROJECT_NAME})
include(GNUInstallDirs)
install(TARGETS ${MAIN} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

enable_testing()
add_subdirectory(test)
