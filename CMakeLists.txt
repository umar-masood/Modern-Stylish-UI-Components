cmake_minimum_required(VERSION 3.16)
project(QtNovaUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED Core Gui Widgets)

add_executable(QtNovaUI
    # UI Components
    components/Button.cpp
    components/CheckBox.cpp
    components/ComboBox.cpp
    components/Delegate.cpp
    components/Dialog.cpp
    components/LineProgress.cpp
    components/Menu.cpp
    components/Rectangle.cpp
    components/RoundedBox.cpp
    components/ScrollBar.cpp
    components/SmoothOpacity.cpp
    components/SmoothShadow.cpp
    components/SpinnerProgress.cpp
    components/Text.cpp
    components/TextField.cpp
    components/Toggle.cpp
    components/ToolTip.cpp
    components/Seperator.cpp

    # Entry Point of App
    # main.cpp

    # Resources
    resources/resources.qrc
)

target_link_libraries(QtNovaUI PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
set_target_properties(QtNovaUI PROPERTIES WIN32_EXECUTABLE TRUE)