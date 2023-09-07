cmake_minimum_required(VERSION 3.27.4)

project(TextSearch)

set(CMAKE_PREFIX_PATH "D:/Kristina/Qt/5.12.12/mingw73_64")

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    main.cpp
    textsearch.cpp
    searchworker.cpp
    searchtask.cpp
    textsearch.h
    searchworker.h
    searchtask.h
    textsearch.ui 
)
set(UI_FILES
    textsearch.ui
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(TextSearch
    ${SOURCES}
    ${UI_HEADERS}
)
target_link_libraries(TextSearch PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)