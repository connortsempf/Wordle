/**
 * @file HowToPlayModal.hpp
 * @brief How To Play Modal Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the HowToPlayModal class, which
 * represents the how to play instructions in the Wordle UI.
 */


#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include "Model/GameConfig.hpp"
#include "View/Components/Modal.hpp"
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Pop up modal screen component for the how to play content.
 */
class HowToPlayModal : public Modal {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a HowToPlayModal object.
         *
         * @param parent The parent QWidget.
         */
        explicit HowToPlayModal(QWidget *parent = nullptr);

    private:
        /**
         * @brief The tile used to illustrate correct tiles.
         */
        QLabel* correctTile;

        /**
         * @brief The tile used to illustrate present tiles.
         */
        QLabel* presentTile;

        /**
         * @brief The tile used to illustrate absent tiles.
         */
        QLabel* absentTile;

    private:
        /**
         * @brief Animates the highlighted example tiles on opening of the modal.
         */
        void animateExampleTiles();
};
