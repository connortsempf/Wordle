#######################
#### Project Setup ####
#######################

## CMake Version Requirement ##
cmake_minimum_required(VERSION 3.16)

## Author and Developer ##
set(AUTHOR "Connor Sempf")

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




#########################
#### Configure Build ####
#########################

## Default to Release Mode Configuration ##
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()


## Force Debug or Release Mode Configurations ##
if(CMAKE_BUILD_TYPE AND
   NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND
   NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(FATAL_ERROR
        "Invalid build type: ${CMAKE_BUILD_TYPE}.

        Use the Debug or Release configuration.

        \"cmake -DCMAKE_BUILD_TYPE=Debug\"
        or
        \"cmake -DCMAKE_BUILD_TYPE=Release\"
        "
    )
endif()




######################
#### Build Source ####
######################

## Add Header Files ##
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/ViewModel/WordleApp.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/WordleEngine.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/WordleSettings.hpp"
    "${CMAKE_SOURCE_DIR}/include/Model/LoggingManager.hpp"
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
    "${CMAKE_SOURCE_DIR}/src/Model/WordleSettings.cpp"
    "${CMAKE_SOURCE_DIR}/src/Model/LoggingManager.cpp"
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

## Tell the Application the Build Configuration ##
target_compile_definitions(Wordle PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)

## Target Qt Resources Files ##
target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/resources.qrc")

## Target Include Files ##
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

## Link to Qt Framework ##
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets)

## Output Binaries to Bin Directory ##
if(WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt REQUIRED)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMENT "Deploying Qt Dependencies to Output Directory..."
    )
endif()




###############################
#### Deploy to OS Platform ####
###############################

## Windows Application Build ##
if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}.rc")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
    install(CODE "
        execute_process(
            COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
                    \"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.exe\"
        )
    ")
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
    set_source_files_properties("${CMAKE_SOURCE_DIR}/resources/macOS/${PROJECT_NAME}.icns"
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/platform/macOS/${PROJECT_NAME}.icns")
    install(TARGETS ${PROJECT_NAME} DESTINATION Applications)
endif()

## Linux Application Build ##
if(UNIX AND NOT APPLE)
    install(TARGETS ${PROJECT_NAME} DESTINATION bin)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/${PROJECT_NAME}.desktop" DESTINATION share/applications)
    install(FILES "${CMAKE_SOURCE_DIR}/platform/linux/${PROJECT_NAME}.png" DESTINATION share/pixmaps)
endif()




##################################
#### Package for Installation ####
##################################

## Set Packaging Configurations ##
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/packages")
set(CPACK_PACKAGE_VENDOR "${AUTHOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Word Guessing Game")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/EULA.txt")
set(CPACK_VERBATIM_VARIABLES YES)

## Windows Packaging ##
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    ## MSI Installation Specifications ##
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_UPGRADE_GUID "64093d62-e197-2dce-b77f-cccb269fd4e0")
    set(CPACK_WIX_PROGRAM_MENU_FOLDER "${PROJECT_NAME}")
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}.ico")
    set(CPACK_WIX_UI_DIALOG "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}-Dialog.bmp")
    set(CPACK_WIX_UI_BANNER "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}-Banner.bmp")
    set_property(INSTALL "$<TARGET_FILE_NAME:${PROJECT_NAME}>" PROPERTY CPACK_START_MENU_SHORTCUTS "${PROJECT_NAME}")
    set_property(INSTALL "$<TARGET_FILE_NAME:${PROJECT_NAME}>" PROPERTY CPACK_DESKTOP_SHORTCUTS "${PROJECT_NAME}")
    ## EXE (NSIS) Installation Specifications ##
    set(CPACK_NSIS_MODIFY_PATH OFF)
    set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
    set(CPACK_NSIS_UNINSTALL_NAME "Uninstall-${PROJECT_NAME}")
    set(CPACK_NSIS_WELCOME_TITLE "Welcome to ${PROJECT_NAME} Installation!")
    set(CPACK_NSIS_FINISH_TITLE "${PROJECT_NAME} Installation Complete!")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_NAME}.exe")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}.ico")
    # set(CPACK_NSIS_MUI_WELCOMEFINISHPAGE_BITMAP "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}-Dialog-Welcome.bmp")
    # set(CPACK_NSIS_MUI_UNWELCOMEFINISHPAGE_BITMAP "${CMAKE_SOURCE_DIR}/platform/windows/${PROJECT_NAME}-Dialog-Welcome.bmp")
    set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\${PROJECT_NAME}.exe'")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk'")
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
        "Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Uninstall.lnk'
        CreateShortCut '$DESKTOP\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\${PROJECT_NAME}.exe'")
    set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
        "Delete '$DESKTOP\\\\${PROJECT_NAME}.lnk'
        Delete '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk'
        RMDir '$SMPROGRAMS\\\\$STARTMENU_FOLDER'")
endif()

## macOS Packaging ##
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR "DragNDrop")
    # DMG Installation Specifications ##
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/platform/macOS/${PROJECT_NAME}.icns")
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/platform/macOS/${PROJECT_NAME}.png")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
endif()

## Linux Packaging ##
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR "DEB;RPM")
    ## DEB Installation Specifications ##
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utilities")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    ## RPM Installation Specifications ##
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase >= 6.0")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Utilities")
endif()

## Include CPack Module ##
include(CPack)
