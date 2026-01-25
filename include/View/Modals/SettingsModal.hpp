/**
 * @file SettingsModal.hpp
 * @brief Settings Modal Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the SettingsModal class, which
 * represents the settings modal in the Wordle UI.
 */


#pragma once
#include <array>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include "View/Components/Switch.hpp"
#include "View/Components/Modal.hpp"
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Pop up modal screen component for the settings content.
 */
class SettingsModal : public Modal {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a SettingsModal object.
         *
         * @param parent The parent QWidget.
         */
        explicit SettingsModal(QWidget *parent = nullptr);

        /**
         * @brief .
         *
         * @param isChecked .
         */
        void setHardModeSwitch(bool isChecked);

        /**
         * @brief .
         *
         * @param isChecked .
         */
        void setDarkThemeSwitch(bool isChecked);

        /**
         * @brief .
         *
         * @param isChecked .
         */
        void setHighContrastModeSwitch(bool isChecked);

        /**
         * @brief .
         *
         * @param isChecked .
         */
        void setOnScreenKeyboardInputOnlySwitch(bool isChecked);

        /**
         * @brief Enable the ability to toggle hard mode switch on and off.
         */
        void enableHardModeToggling();

        /**
         * @brief Disable the ability to toggle hard mode switch on and off.
         */
        void disableHardModeToggling();

    signals:
        /**
         * @brief Signal emitted when the modal enables hard mode.
         */
        void enableHardMode();

        /**
         * @brief Signal emitted when the modal enables dark theme.
         */
        void enableDarkTheme();

        /**
         * @brief Signal emitted when the modal enables high contrast mode.
         */
        void enableHighContrastMode();

        /**
         * @brief Signal emitted when the modal enables hard mode.
         */
        void enableOnscreenKeyboardInputOnly();

        /**
         * @brief Signal emitted when the modal disables hard mode.
         */
        void disableHardMode();

        /**
         * @brief Signal emitted when the modal disables dark theme.
         */
        void disableDarkTheme();

        /**
         * @brief Signal emitted when the modal disables high contrast mode.
         */
        void disableHighContrastMode();

        /**
         * @brief Signal emitted when the modal disables hard mode.
         */
        void disableOnscreenKeyboardInputOnly();

        /**
         * @brief Signal emitted when the switch has been pressed but is disabled.
         */
        void disabledSwitchPressed();

    private:
        /**
         * @brief Settings switches.
         */
        std::array<Switch*, 4> settingsSwitches;
};
