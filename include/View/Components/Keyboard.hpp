/**
 * @file Keyboard.hpp
 * @brief Keyboard Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the Keyboard class, which
 * represents the Wordle UI keyboard.
 */


#pragma once
#include <string>
#include <array>
#include <vector>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QChar>
#include "Model/LetterState.hpp"
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Represents a single button in the Wordle UI keyboard.
 */
class Keyboard : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Represents the visual state of the keyboard buttons.
         */
        enum class ButtonVisualState { ABSENT, PRESENT, CORRECT };

    public:
        /**
         * @brief Constructs a Keyboard object.
         *
         * @param parent The parent QWidget.
         */
        explicit Keyboard(QWidget *parent = nullptr);

        /**
         * @brief Reset the logical and visual state for all key buttons in the keyboard.
         */
        void reset();

        /**
         * @brief Set the logical state for a key button.
         *
         * @param keyLetter The key letter.
         * @param letterState The visual state to set (ABSENT, PRESENT, CORRECT).
         */
        void setKeyState(QChar keyLetter, LetterState letterState);

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

    private:
        /**
         * @brief Committed state for the key button (whether it displays the letter state color or base color).
         */
        enum class CommitState { UNCOMMITTED, COMMITTED };

    private:
        /**
         * @brief The width of each key button.
         */
        static const unsigned int KEY_WIDTH;

        /**
         * @brief The height of each key button.
         */
        static const unsigned int KEY_HEIGHT;

        /**
         * @brief The spacing between key buttons.
         */
        static const unsigned int KEY_SPACING;

        /**
         * @brief The width of the function key buttons.
         */
        static const unsigned int FUNCTION_KEY_WIDTH;

        /**
         * @brief The layout of the keyboard button keys.
         */
        static const std::vector<std::vector<QString>> KEY_BUTTON_TEXTS;

    private:
        /**
         * @brief Keyboard key buttons and their committed and letter indicator state.
         */
        std::vector<std::vector<std::tuple<QPushButton*, CommitState, LetterState>>> keys;

    private:
        /**
         * @brief Update the visual style for a given key button.
         *
         * @param key The key button to update.
         */
        void updateStyle(const std::tuple<QPushButton*, CommitState, LetterState>& key);
};
