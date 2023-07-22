cmake_minimum_required(VERSION 3.25)
project(kde-wine-helper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(KF5_MIN_VERSION "5.88.0")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Qt5 COMPONENTS
        Core
        DBus
        REQUIRED)
find_package(KF5 REQUIRED COMPONENTS WindowSystem)

set(CONFIG_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config.json")
configure_file(${CONFIG_FILE_PATH} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/icons")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/icons/app.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/icons")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/icons/transparent.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/icons")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/icons/wechat.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/icons")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/icons/wxwork.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/icons")

add_executable(kde-wine-helper main.cpp XWindowListener.h Notifier.h Notifier.cpp XWindowListener.cpp Config.h)
target_link_libraries(kde-wine-helper
        Qt5::Core
        Qt5::DBus
        KF5::WindowSystem
        )

