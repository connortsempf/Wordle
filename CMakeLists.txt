#######################
#### Project Setup ####
#######################

## CMake Version Requirement ##
cmake_minimum_required(VERSION 3.16)

## Project Definition ##
project(Wordle VERSION 1.0.0 LANGUAGES CXX)

## C++ Language Version ##
set(CMAKE_CXX_STANDARD 20)

## Require C++ Language Version ##
set(CMAKE_CXX_STANDARD_REQUIRED ON)




####################
#### Qt Package ####
####################

## Enable Qt MOC ##
set(CMAKE_AUTOMOC ON)

## Enable Qt UIC ##
set(CMAKE_AUTOUIC ON)

## Enable Qt RCC ##
set(CMAKE_AUTORCC ON)

## MinGW RC Compiler Setup for Windows RC File in Build ##
if(MINGW)
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

## Get the Qt Package ##
option(QT6_DIR "Path to Local Qt Compiler Installation" "")
if(QT6_DIR)
    find_package(Qt6 REQUIRED COMPONENTS Widgets PATHS "${QT6_DIR}" NO_DEFAULT_PATH)
else()
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    if(NOT Qt6_FOUND)
        message(FATAL_ERROR "Qt6 not found!

        Please install Qt6 development libraries:

        Windows:
        Download from https://www.qt.io/download

        macOS:
        brew install qt6

        Linux (Ubuntu/Debian):
        sudo apt install qt6-base-dev qt6-tools-dev

        Or set QT6_DIR to your Qt compiler-specific libraries directory:
        cmake .. -DQT6_DIR=\"/path/to/your/Qt/package/6.10.x/compiler/\"")
    endif()
endif()




######################
#### Build Source ####
######################

## Add Header Files ##
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/ViewModel/WordleApp.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/WordleEngine.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/WordleStatistics.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/LetterState.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/WordleUI.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/GuessBoard.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/Header.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/Keyboard.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/Modal.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/PopUpMessenger.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Components/Switch.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Modals/StatisticsModal.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Modals/HowToPlayModal.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Modals/SettingsModal.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Screens/IntroScreen.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Screens/GameScreen.hpp"
    "${CMAKE_SOURCE_DIR}/include/View/Themes/ColorTheme.hpp"
)

## Add Source Files ##
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/ViewModel/WordleApp.cpp"
    "${CMAKE_SOURCE_DIR}/src/Model/WordleEngine.cpp"
    "${CMAKE_SOURCE_DIR}/src/Model/WordleStatistics.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/WordleUI.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/GuessBoard.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/Header.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/Keyboard.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/Modal.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/PopUpMessenger.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Components/Switch.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Modals/StatisticsModal.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Modals/HowToPlayModal.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Modals/SettingsModal.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Screens/IntroScreen.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Screens/GameScreen.cpp"
    "${CMAKE_SOURCE_DIR}/src/View/Themes/ColorTheme.cpp"
)

## Generate Binary Files in Bin Directory //
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

## Generate OS-Specific Wordle Application Executable ##
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

## Target Include Files ##
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

## Link to Qt Framework ##
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)

## Output Binaries to Bin Directory ##
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt REQUIRED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe"
        COMMENT "Deploying Qt Dependencies to Output Directory..."
    )
endif()




###############################
#### Deploy to OS Platform ####
###############################

## Windows Application Build ##
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/platform/windows/wordle.rc")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif()

## macOS Application Build ##
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.connorsempf.wordle"
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_ICON_FILE "wordle.icns"
    )
    set_source_files_properties("${CMAKE_SOURCE_DIR}/resources/macOS/wordle.icns"
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

## Linux Application Build ##
if(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/wordle.desktop" DESTINATION share/applications)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/wordle.png" DESTINATION share/pixmaps)
endif()
