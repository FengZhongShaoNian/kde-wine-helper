cmake_minimum_required(VERSION 3.25)
project(KDEWineHelper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(KF5_MIN_VERSION "5.88.0")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt5 COMPONENTS
        Core
        REQUIRED)
find_package(KF5 REQUIRED COMPONENTS WindowSystem)

set(IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/icons/app.png")
configure_file(${IMAGE_PATH} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(KDEWineHelper main.cpp XWindowListener.h Notifier.h Notifier.cpp XWindowListener.cpp)
target_link_libraries(KDEWineHelper
        Qt5::Core
        KF5::WindowSystem
        )

