/**
 * @file Header.hpp
 * @brief Header Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the Header class, which
 * represents the Wordle UI header.
 */


#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPainter>
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Represents a single button in the Wordle UI header.
 */
class Header : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a Header object.
         *
         * @param parent The parent QWidget.
         */
        explicit Header(QWidget *parent = nullptr);

    signals:
        /**
         * @brief Signal emitted when the statistics button is pressed.
         */
        void openStatistics();

        /**
         * @brief Signal emitted when the how to play button is pressed.
         */
        void openHowToPlay();

        /**
         * @brief Signal emitted when the settings button is pressed.
         */
        void openSettings();

    private:
        /**
         * @brief The width of each header button.
         */
        static const unsigned int BUTTON_WIDTH;

        /**
         * @brief The height of each header button.
         */
        static const unsigned int BUTTON_HEIGHT;
};
