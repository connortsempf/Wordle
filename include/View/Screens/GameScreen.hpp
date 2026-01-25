/**
 * @file GameScreen.hpp
 * @brief GameScreen Screen Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the GameScreen class, which represents
 * the entire screen and scene for the GameScreen Screen within the Wordle App.
 */


#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QKeyEvent>
#include "Model/GameConfig.hpp"
#include "Model/WordleEngine.hpp"
#include "View/Components/Header.hpp"
#include "View/Components/GuessBoard.hpp"
#include "View/Components/Keyboard.hpp"


/**
 * @brief The GameScreen Screen UI class.
 */
class GameScreen : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a GameScreen object.
         *
         * @param parent The parent QWidget.
         */
        explicit GameScreen(QWidget *parent = nullptr);

        /**
         * @brief Reset the visual state of the UI for a new game.
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
         */
        void commitGuess(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates);

        /**
         * @brief Sets the state of a keyboard key button.
         *
         * @param keyLetter The key letter.
         * @param letterState The state to set (ABSENT, PRESENT, CORRECT).
         */
        void setKeyboardKeyState(QChar keyLetter, LetterState letterState);

        /**
         * @brief Response to an invalid guess occurrance.
         */
        void invalidGuess();

    signals:
        /**
         * @brief Signal emitted when a letter key input occurs.
         *
         * @param letter The letter that was pressed.
         */
        void letterInput(QChar letter);

        /**
         * @brief Signal emitted when the enter key input occurs.
         */
        void enterInput();

        /**
         * @brief Signal emitted when the backspace key input occurs.
         */
        void backspaceInput();

        /**
         * @brief Signal emitted when the Statistics Button is pressed.
         */
        void openStatistics();

        /**
         * @brief Signal emitted when the How To Play Button is pressed.
         */
        void openHowToPlay();

        /**
         * @brief Signal emitted when the Settings Button is pressed.
         */
        void openSettings();

        /**
         * @brief Signal emitted when the animation for the commitment of all letters finishes in a guess row.
         */
        void commitLettersAnimationFinished();

    private:
        /**
         * @brief The Guess Board UI component.
         */
        GuessBoard* guessBoard;

        /**
         * @brief The Keyboard UI component.
         */
        Keyboard* keyboard;
};
