/**
 * @file GameScreen.cpp
 * @brief GameScreen Screen Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the GameScreen class, which represents
 * the entire screen and scene for the GameScreen Screen within the Wordle App.
 */


#include "View/Screens/GameScreen.hpp"


/**
 * @brief Constructs a GameScreen object.
 *
 * @param parent The parent QWidget.
 */
GameScreen::GameScreen(QWidget *parent) : QWidget(parent) {
    // Construct Components //
    QVBoxLayout *layout = new QVBoxLayout(this);
    Header* header = new Header(this);
    guessBoard = new GuessBoard(this);
    keyboard = new Keyboard(this);

    // Connect and Propagate Signals //
    connect(header, &Header::openStatistics, this, &GameScreen::openStatistics);
    connect(header, &Header::openHowToPlay, this, &GameScreen::openHowToPlay);
    connect(header, &Header::openSettings, this, &GameScreen::openSettings);
    connect(keyboard, &Keyboard::letterInput, this, &GameScreen::letterInput);
    connect(keyboard, &Keyboard::enterInput, this, &GameScreen::enterInput);
    connect(keyboard, &Keyboard::backspaceInput, this, &GameScreen::backspaceInput);

    // Connect GuessBoard Signals //
    connect(guessBoard, &GuessBoard::commitLettersAnimationFinished, this, &GameScreen::commitLettersAnimationFinished);

    // Layout Components //
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(header);
    layout->addStretch();
    layout->addWidget(guessBoard);
    layout->addStretch();
    layout->addWidget(keyboard);
    setLayout(layout);
};


/**
 * @brief Reset the visual state of the UI for a new game.
 */
void GameScreen::reset() {
    guessBoard->reset();
    keyboard->reset();
}


/**
 * @brief Appends a letter to the current row.
 *
 * @param letter The letter to append.
 */
void GameScreen::appendLetter(QChar letter) {
    guessBoard->appendLetter(letter);
}


/**
 * @brief Deletes a letter from the current row.
 */
void GameScreen::deleteLetter() {
    guessBoard->deleteLetter();
}


/**
 * @brief Commits the letters of a guess in the row with their respective validities.
 *
 * @param letterStates An array of letter states for each tile.
 */
void GameScreen::commitGuess(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates) {
    guessBoard->commitGuess(letterStates);
}


/**
 * @brief Sets the state of a keyboard key button.
 *
 * @param keyLetter The key letter.
 * @param letterState The state to set (ABSENT, PRESENT, CORRECT).
 */
void GameScreen::setKeyboardKeyState(QChar keyLetter, LetterState letterState) {
    keyboard->setKeyState(keyLetter, letterState);
}


/**
 * @brief Response to an invalid guess occurrance.
 */
void GameScreen::invalidGuess() {
    guessBoard->invalidGuess();
}
