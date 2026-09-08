##################################
#### Package for Installation ####
##################################




######################################
#### Set Packaging Configurations ####
######################################

set(CPACK_PACKAGE_CONTACT "${AUTHOR} ${AUTHOR_CONTACT}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_SOURCE_DIR}/packages")
set(CPACK_PACKAGE_VENDOR "${AUTHOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A Word Guessing Game")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/EULA.txt")
set(CPACK_VERBATIM_VARIABLES YES)




###########################
#### Windows Packaging ####
###########################

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(CPACK_GENERATOR "WIX;NSIS")

    ## MSI Installation Specifications ##
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
    set(CPACK_NSIS_MUI_WELCOMEFINISHPAGE_BITMAP "${CMAKE_SOURCE_DIR}\\platform\\windows\\${PROJECT_NAME}-Dialog-Welcome.bmp")
    set(CPACK_NSIS_MUI_UNWELCOMEFINISHPAGE_BITMAP "${CMAKE_SOURCE_DIR}\\platform\\windows\\${PROJECT_NAME}-Dialog-Unwelcome.bmp")
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




#########################
#### macOS Packaging ####
#########################

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR "DragNDrop")
    
    # DMG Installation Specifications ##
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/platform/macOS/${PROJECT_NAME}.icns")
    set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_SOURCE_DIR}/platform/macOS/${PROJECT_NAME}.png")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
endif()




#########################
#### Linux Packaging ####
#########################

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




##############################
#### Include CPack Module ####
##############################

include(CPack)