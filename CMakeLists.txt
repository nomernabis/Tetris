cmake_minimum_required(VERSION 3.2)
project(Tetris)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Sources/Game.cpp Headers/Game.h Utils/Constants.h UI/ControlBox.cpp UI/ControlBox.h UI/Command.h UI/InfoScreen.cpp UI/InfoScreen.h Utils/Shapes.h Utils/Shapes.cpp Sources/Shape.cpp Headers/Shape.h Headers/MessageInterface.h)
add_executable(Tetris ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Tetris ${SFML_LIBRARIES})
endif()