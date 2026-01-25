/**
 * @file StatisticsModal.hpp
 * @brief Statistics Modal Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the StatisticsModal class, which
 * represents the statistics in the Wordle UI.
 */


#pragma once
#include <algorithm>
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
#include "Model/GameConfig.hpp"
#include "Model/WordleStatistics.hpp"
#include "View/Components/Modal.hpp"
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Pop up modal screen component for the statistics content.
 */
class StatisticsModal : public Modal {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a StatisticsModal object.
         *
         * @param parent The parent QWidget.
         */
        explicit StatisticsModal(QWidget *parent = nullptr);

        /**
         * @brief Set the statistics metrics for the UI to display.
         *
         * @param statistics The statistics to display.
         */
        void setStatistics(WordleStatistics::Statistics statistics);

        /**
         * @brief Enable the ability to restart from the Statistics Modal.
         */
        void enableRestart();

        /**
         * @brief Disable the ability to restart from the Statistics Modal.
         */
        void disableRestart();

    signals:
        /**
         * @brief Signal emitted when the modal desires starting a new game.
         */
        void playAgain();

    private:
        /**
         * @brief Header stat labels.
         */
        std::array<QLabel*, 4> headerStatLabels;

        /**
         * @brief Guess distribution stats layout.
         */
        QVBoxLayout* guessDistributionsStatsLayout;

        /**
         * @brief Guess distribution text bars.
         */
        std::array<QLabel*, GameConfig::NUM_GUESSES> guessDistributionBars;

        /**
         * @brief Play again restart button layout.
         */
        QHBoxLayout* playAgainButtonLayout;
};
