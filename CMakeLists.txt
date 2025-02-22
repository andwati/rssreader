cmake_minimum_required(VERSION 3.14)
project(rss_reader VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(SOURCES
        src/main.cpp
        src/main_window.cpp
        src/feed_parser.cpp
)

set(HEADERS
        include/feed.hpp
        include/feed_parser.hpp
        include/main_window.hpp
        src/main_window.cpp
        src/feed_parser.cpp
        include/main_window.hpp
        include/feed_parser.hpp
        include/feed.hpp
)

add_executable(rss_reader ${SOURCES} ${HEADERS})
target_include_directories(rss_reader PRIVATE include)

target_link_libraries(rss_reader PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)