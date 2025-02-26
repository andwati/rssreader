cmake_minimum_required(VERSION 3.14)
project(rss_reader VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

set(RESOURCES
        resources/resources.qrc
)

set(SOURCES
        src/main.cpp
        src/main_window.cpp
        src/feed_parser.cpp
        src/storage.cpp
        src/settings_dialog.cpp
)

set(HEADERS
        include/feed.hpp
        include/feed_parser.hpp
        include/main_window.hpp
        include/storage.hpp
        include/settings_dialog.hpp
)

add_executable(rss_reader ${SOURCES} ${HEADERS} ${RESOURCES})
target_include_directories(rss_reader PRIVATE include)

target_link_libraries(rss_reader PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)

include(GNUInstallDirs)

install(TARGETS rss_reader
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/rss_reader.desktop.ini
        ${CMAKE_CURRENT_BINARY_DIR}/rss_reader.desktop
        @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rss_reader.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
)

install(FILES resources/icons/rss_reader.png
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/128x128/apps
)