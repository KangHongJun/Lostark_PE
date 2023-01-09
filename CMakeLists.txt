cmake_minimum_required(VERSION 3.5)
set(CMAKE_PREFIX_PATH "E:/Qt/6.4.1/mingw_64/lib/cmake/")
set(BOOSTROOT "E:/boost-1.81.0/")

set(CURL "E:/curl-7.87.0/")


set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(Lostark_PE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Console)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS)
find_package(CURL REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIR})
include_directories(${CURL_SOURCE_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.ui
        mainwindow.cpp mainwindow.h
        VProductList.cpp VProductList.h
        widget.cpp widget.h
        finddialog.cpp finddialog.h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Lostark_PE
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Lostark_PE APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Lostark_PE SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Lostark_PE
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Lostark_PE PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
        ws2_32
        ${CURL_LIBRARIES}
       )
target_link_libraries(Lostark_PE ${Boost_FILESYSTEM_LIBRARY})


set_target_properties(Lostark_PE PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Lostark_PE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Lostark_PE)
endif()
