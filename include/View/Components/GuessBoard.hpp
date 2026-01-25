/**
 * @file GuessBoard.hpp
 * @brief Guess Board Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the GuessBoard class, which
 * represents a single row of tiles in the Wordle UI.
 */


#pragma once
#include <array>
#include <tuple>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include "Model/LetterState.hpp"
#include "View/Themes/ColorTheme.hpp"
#include "Model/GameConfig.hpp"


/**
 * @brief Represents the playing board of guess rows of tiles in the Wordle UI.
 */
class GuessBoard : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Represents the state of a tile.
         */
        enum class TileState { EMPTY, FILLED, COMMITTED };

    public:
        /**
         * @brief Constructs a GuessBoard object.
         *
         * @param parent The parent QWidget.
         */
        explicit GuessBoard(QWidget *parent = nullptr);

        /**
         * @brief Reset the logical and visual state for a new game.
         */
        void reset();

        /**
         * @brief Appends a letter to the current row.
         *
         * @param letter The letter to append.
         */
        void appendLetter(QChar letter);

        /**
         * @brief Deletes a letter from the current row.
         */
        void deleteLetter();

        /**
         * @brief Commits the letters of a guess in the row with their respective validities.
         *
         * @param letterStates An array of letter states for each tile.
         *
         * @return True if the commit was successful, false otherwise.
         */
        void commitGuess(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates);

        /**
         * @brief Response to an invalid guess occurrance.
         */
        void invalidGuess();

    signals:
        /**
         * @brief Signal emitted when the animation for the commitment of all letters finishes in a guess row.
         */
        void commitLettersAnimationFinished();

    private:
        /**
         * @brief Amount that each side of the tiles expand during append letter animations.
         */
        static const unsigned int animateAppendLetterExpansion = 4;

    private:
        /**
         * @brief The currently active letter tile row in the game.
         */
        unsigned int activeRow = 0;

        /**
         * @brief The currently active letter tile column in the game.
         */
        unsigned int activeColumn = 0;

        /**
         * @brief Shake animation.
         */
        QParallelAnimationGroup* shakeAnimation = nullptr;

        /**
         * @brief The letter tiles and their associated tile and letter states.
         */
        std::array<std::array<std::tuple<QLabel*, TileState, LetterState>, GameConfig::WORD_LENGTH>, GameConfig::NUM_GUESSES> letterTiles;

    private:
        /**
         * @brief Updates the visual style of the tile based on its state.
         *
         * @param letterTile The letter tile to update.
         */
        void updateLetterTileStyle(std::tuple<QLabel*, TileState, LetterState> letterTile);

        /**
         * @brief Animate the letter tile when a letter is appended.
         *
         * @param letterTile The letter tile to animate.
         * @param activeColumn The active column in the active row.
         */
        void animateAppendLetter(std::tuple<QLabel*, TileState, LetterState> letterTile, unsigned int activeColumn);

        /**
         * @brief Animate the active row tiles when an invalid guess occurs.
         *
         * @param activeRow The active row to animate.
         */
        void animateInvalidGuess(unsigned int activeRow);

        /**
         * @brief Animate the letter tiles of the active row when letters are committed.
         *
         * @param letterStates The states of committed letters.
         * @param activeRow The active row of the letter commitment.
         */
        void animateCommitLetters(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates, unsigned int activeRow);

        /**
         * @brief Animate the active row tiles when the guess is correct.
         *
         * @param activeRow The active row to animate.
         */
        void animateCorrectGuess(unsigned int activeRow);
};
