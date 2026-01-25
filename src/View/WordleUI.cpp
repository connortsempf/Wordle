/**
 * @file WordleUI.cpp
 * @brief Wordle UI Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the WordleUI class, which
 * manages the state and coordination of the Wordle UI.
 */


#include "View/WordleUI.hpp"


/**
 * @brief Constructs a WordleUI object.
 *
 * @param parent The parent QWidget.
 */
WordleUI::WordleUI(QWidget *parent) : QWidget(parent) {
    // Construct Screen and Modal Components //
    introScreen = new IntroScreen(this);
    gameScreen = new GameScreen(this);
    statisticsModal = new StatisticsModal(this);
    howToPlayModal = new HowToPlayModal(this);
    settingsModal = new SettingsModal(this);
    popUpMessenger = new PopUpMessenger(this);

    // Construct Layout //
    screenStack = new QStackedLayout(this);
    screenStack->setStackingMode(QStackedLayout::StackAll);
    screenStack->setContentsMargins(0, 0, 0, 0);
    screenStack->addWidget(introScreen);
    screenStack->addWidget(popUpMessenger);
    screenStack->addWidget(statisticsModal);
    screenStack->addWidget(howToPlayModal);
    screenStack->addWidget(settingsModal);
    screenStack->addWidget(gameScreen);

    // Connect and UI Interaction Handle Signals //
    connect(introScreen,        &IntroScreen::playInput,                            this, &WordleUI::playGameInput);
    connect(gameScreen,         &GameScreen::letterInput,                           this, &WordleUI::keyboardLetterInput);
    connect(gameScreen,         &GameScreen::enterInput,                            this, &WordleUI::keyboardEnterInput);
    connect(gameScreen,         &GameScreen::backspaceInput,                        this, &WordleUI::keyboardBackspaceInput);
    connect(gameScreen,         &GameScreen::openStatistics,                        this, &WordleUI::openStatisticsInput);
    connect(gameScreen,         &GameScreen::openHowToPlay,                         this, &WordleUI::openHowToPlayInput);
    connect(gameScreen,         &GameScreen::openSettings,                          this, &WordleUI::openSettingsInput);
    connect(gameScreen,         &GameScreen::commitLettersAnimationFinished,        this, &WordleUI::commitLettersAnimationFinished);
    connect(statisticsModal,    &StatisticsModal::close,                            this, &WordleUI::closeStatisticsInput);
    connect(howToPlayModal,     &HowToPlayModal::close,                             this, &WordleUI::closeHowToPlayInput);
    connect(settingsModal,      &SettingsModal::close,                              this, &WordleUI::closeSettingsInput);
    connect(statisticsModal,    &StatisticsModal::playAgain,                        this, &WordleUI::playAgainInput);
    connect(settingsModal,      &SettingsModal::enableHardMode,                     this, &WordleUI::enableHardMode);
    connect(settingsModal,      &SettingsModal::enableDarkTheme,                    this, &WordleUI::enableDarkTheme);
    connect(settingsModal,      &SettingsModal::enableHighContrastMode,             this, &WordleUI::enableHighContrastMode);
    connect(settingsModal,      &SettingsModal::enableOnscreenKeyboardInputOnly,    this, &WordleUI::enableOnscreenKeyboardInputOnly);
    connect(settingsModal,      &SettingsModal::disableHardMode,                    this, &WordleUI::disableHardMode);
    connect(settingsModal,      &SettingsModal::disableDarkTheme,                   this, &WordleUI::disableDarkTheme);
    connect(settingsModal,      &SettingsModal::disableHighContrastMode,            this, &WordleUI::disableHighContrastMode);
    connect(settingsModal,      &SettingsModal::disableOnscreenKeyboardInputOnly,   this, &WordleUI::disableOnscreenKeyboardInputOnly);

    // Set Theme //
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this]() {
        const auto& theme = ColorTheme::instance().getTheme();
        QColor backgroundColor = theme.background.background;
        QString style = QString("background-color: rgb(%1, %2, %3);").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
        this->setStyleSheet(style);
    });
    connect(settingsModal, &SettingsModal::disabledSwitchPressed, this, &WordleUI::disabledSwitchPressed);

    // Configure UI //
    setLayout(screenStack);

}


/**
 * @brief .
 */
void WordleUI::openHowToPlay() {
    howToPlayModal->show();
}


/**
 * @brief .
 */
void WordleUI::openStatistics() {
    statisticsModal->show();
}


/**
 * @brief .
 */
void WordleUI::openSettings() {
    settingsModal->show();
}


/**
 * @brief .
 */
void WordleUI::closeIntroScreen() {
    introScreen->hide();
    howToPlayModal->hide();
    statisticsModal->hide();
    settingsModal->hide();
}


/**
 * @brief .
 */
void WordleUI::closeHowToPlay() {
    howToPlayModal->hide();
}


/**
 * @brief .
 */
void WordleUI::closeStatistics() {
    statisticsModal->hide();
}


/**
 * @brief .
 */
void WordleUI::closeSettings() {
    settingsModal->hide();
}


/**
 * @brief .
 *
 * @param statistics .
 */
void WordleUI::setStatistics(WordleStatistics::Statistics statistics) {
    statisticsModal->setStatistics(statistics);
}


/**
 * @brief .
 */
void WordleUI::enableRestart() {
    statisticsModal->enableRestart();
}


/**
 * @brief .
 */
void WordleUI::disableRestart() {
    statisticsModal->disableRestart();
}


/**
 * @brief .
 */
void WordleUI::resetGameUI() {
    gameScreen->reset();
}


/**
 * @brief .
 *
 * @param letter .
 */
void WordleUI::appendLetter(QChar letter) {
    gameScreen->appendLetter(letter);
}


/**
 * @brief .
 */
void WordleUI::deleteLetter() {
    gameScreen->deleteLetter();
}


/**
 * @brief .
 *
 * @param letterStates .
 */
void WordleUI::commitGuess(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates) {
    gameScreen->commitGuess(letterStates);
}


/**
 * @brief .
 *
 * @param keyLetter .
 * @param letterState .
 */
void WordleUI::setKeyboardKeyState(QChar keyLetter, LetterState letterState) {
    gameScreen->setKeyboardKeyState(keyLetter, letterState);
}


/**
 * @brief .
 */
void WordleUI::invalidGuess() {
    gameScreen->invalidGuess();
}


/**
 * @brief .
 *
 * @param message .
 */
void WordleUI::addPopUpMessage(QString message) {
    popUpMessenger->addMessage(message);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void WordleUI::setHardModeSwitch(bool isChecked) {
    settingsModal->setHardModeSwitch(isChecked);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void WordleUI::setDarkThemeSwitch(bool isChecked) {
    settingsModal->setDarkThemeSwitch(isChecked);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void WordleUI::setHighContrastModeSwitch(bool isChecked) {
    settingsModal->setHighContrastModeSwitch(isChecked);
}

/**
 * @brief .
 *
 * @param isChecked .
 */
void WordleUI::setOnScreenKeyboardInputOnlySwitch(bool isChecked) {
    settingsModal->setOnScreenKeyboardInputOnlySwitch(isChecked);
}


/**
 * @brief Enable the ability to toggle hard mode switch on and off.
 */
void WordleUI::enableHardModeToggling() {
    settingsModal->enableHardModeToggling();
}


/**
 * @brief Disable the ability to toggle hard mode switch on and off.
 */
void WordleUI::disableHardModeToggling() {
    settingsModal->disableHardModeToggling();
}
