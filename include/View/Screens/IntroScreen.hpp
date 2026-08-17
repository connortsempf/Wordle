/**
 * @file IntroScreen.hpp
 * @brief IntroScreen Screen Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the IntroScreen class, which represents
 * the entire screen and scene for the IntroScreen Screen within the Wordle App.
 */


#pragma once
#include <QWidget>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "Model/GameConfig.hpp"


/**
 * @brief The IntroScreen Screen UI class.
 */
class IntroScreen : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a IntroScreen object.
         *
         * @param parent The parent QWidget.
         */
        explicit IntroScreen(QWidget *parent = nullptr);

    signals:
        /**
         * @brief Signal emitted when a the user wants to play the Wordle game.
         */
        void playInput();
};
