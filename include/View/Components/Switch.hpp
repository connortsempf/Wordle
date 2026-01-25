/**
 * @file Switch.hpp
 * @brief Switch Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the Switch class, which
 * represents a single switch in the Wordle UI.
 */


#pragma once
#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QChar>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Represents a single tile in the Wordle UI.
 */
class Switch : public QCheckBox {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a Switch object.
         *
         * @param parent The parent QWidget.
         */
        explicit Switch(QWidget *parent = nullptr);

    signals:
        /**
         * @brief Signal emitted when the switch is changed from off to on state.
         */
        void switchedOn();

        /**
         * @brief Signal emitted when the switch is changed from on to off state.
         */
        void switchedOff();

        /**
         * @brief Signal emitted when the switch has been pressed but is disabled.
         */
        void disabledSwitchPressed();

    private:
        /**
         * @brief Represents the switchability state of a switch.
         */
        enum class Ability { DISABLED, ENABLED };

        /**
         * @brief Represents the state of a switch.
         */
        enum class State { ON, OFF };

    private:
        /**
         * @brief The thumb component of the switch.
         */
        QWidget* thumb;

    private:
        /**
         * @brief Animates the appending of a letter to the tile.
         */
        void animateSwitching();
};
