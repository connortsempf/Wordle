/**
 * @file WordleUI.hpp
 * @brief Wordle UI Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the WordleUI class, which
 * manages the state and coordination of the Wordle UI.
 */


#pragma once
#include <string>
#include <QWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include "Model/WordleEngine.hpp"
#include "View/Components/Header.hpp"
#include "View/Components/GuessBoard.hpp"
#include "View/Components/Keyboard.hpp"
#include "View/Components/PopUpMessenger.hpp"
#include "View/Components/Modal.hpp"
#include "View/Modals/HowToPlayModal.hpp"
#include "View/Modals/StatisticsModal.hpp"
#include "View/Modals/SettingsModal.hpp"
#include "View/Screens/IntroScreen.hpp"
#include "View/Screens/GameScreen.hpp"
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief The UI managing class for the Wordle game.
 */
class WordleUI : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a WordleUI object.
         *
         * @param parent The parent QWidget.
         */
        explicit WordleUI(QWidget *parent = nullptr);

    public:
        void openHowToPlay();
        void openStatistics();
        void openSettings();
        void closeIntroScreen();
        void closeHowToPlay();
        void closeStatistics();
        void closeSettings();
        void setStatistics(WordleSettings::Statistics statistics);
        void enableRestart();
        void disableRestart();
        void resetGameUI();
        void appendLetter(QChar letter);
        void deleteLetter();
        void commitGuess(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates);
        void setKeyboardKeyState(QChar keyLetter, LetterState letterState);
        void invalidGuess();
        void addPopUpMessage(QString message);
        void setHardModeSwitch(bool isChecked);
        void setDarkThemeSwitch(bool isChecked);
        void setHighContrastModeSwitch(bool isChecked);
        void setOnScreenKeyboardInputOnlySwitch(bool isChecked);
        void enableHardModeToggling();
        void disableHardModeToggling();

    signals:
        void playGameInput();
        void keyboardLetterInput(QChar letter);
        void keyboardEnterInput();
        void keyboardBackspaceInput();
        void openStatisticsInput();
        void openHowToPlayInput();
        void openSettingsInput();
        void closeStatisticsInput();
        void closeHowToPlayInput();
        void closeSettingsInput();
        void playAgainInput();
        void enableHardMode();
        void enableDarkTheme();
        void enableHighContrastMode();
        void enableOnscreenKeyboardInputOnly();
        void disableHardMode();
        void disableDarkTheme();
        void disableHighContrastMode();
        void disableOnscreenKeyboardInputOnly();
        void disabledSwitchPressed();
        void commitLettersAnimationFinished();

    private:
        /**
         * @brief The screen stack for displaying screens and or modals.
         */
        QStackedLayout* screenStack;

        /**
         * @brief The Intro Screen.
         */
        IntroScreen* introScreen;

        /**
         * @brief The Game Screen.
         */
        GameScreen* gameScreen;

        /**
         * @brief The How To Play Modal.
         */
        HowToPlayModal* howToPlayModal;

        /**
         * @brief The Statistics Modal.
         */
        StatisticsModal* statisticsModal;

        /**
         * @brief The Settings Modal.
         */
        SettingsModal* settingsModal;

        /**
         * @brief The Pop Up Message.
         */
        PopUpMessenger* popUpMessenger;
};
