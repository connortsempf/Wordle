/**
 * @file WordleApp.hpp
 * @brief Wordle Application Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the WordleApp class, which
 * manages the overall application state and UI components.
 */


#pragma once
#include <string>
#include <QDebug>
#include <QDateTime>
#include <QTextStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFontDatabase>
#include <QWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include "Model/GameConfig.hpp"
#include "Model/WordleEngine.hpp"
#include "Model/WordleStatistics.hpp"
#include "View/WordleUI.hpp"
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Handle logging for debugging throughout the application.
 *
 * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
 * @param context The context information including file, line, and function where the message originated.
 * @param message The message text to be logged.
 */
void debugLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);


/**
 * @brief The application managing class for the Wordle game.
 */
class WordleApp : public QObject {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a WordleApp object.
         *
         * @param parent The parent QWidget.
         */
        explicit WordleApp(QObject *parent = nullptr);

    protected:
        /**
         * @brief Event filter to capture application events
         */
        bool eventFilter(QObject* obj, QEvent* event) override;

    private:
        /**
         * @brief User Settings struct for persistent app data.
         */
        struct UserSettings {
            QSize windowSize = {625, 800};
            QPoint windowPosition = {-1, -1};
            bool hardMode = false;
            bool darkTheme = true;
            bool highContrastMode = false;
            bool onScreenKeyboardInputOnly = false;
            WordleStatistics::Statistics statistics;
        };

    private:
        /**
         * @brief The current guess being constructed.
         */
        std::string currentGuess = "";

        /**
         * @brief The Wordle game logic manager for the application.
         */
        WordleEngine engine;

        /**
         * @brief The Wordle statistics logic manager for the application.
         */
        WordleStatistics statistics;

        /**
         * @brief The Wordle UI for the application.
         */
        WordleUI* ui;

        /**
         * @brief Flag for controlling whether game-letter input can only come from the onscreen keyboard.
         */
        bool onScreenKeyboardInputOnly = false;

        /**
         * @brief Cache of user settings to track during runtime and save for persistent settings tracking.
         */
        UserSettings userSettings;

        /**
         * @brief Flag for if any game input will be acted upon.
         */
        bool acceptingGameInput = false;

    private:
        /**
         * @brief Load fonts for the application UI to use.
         */
        void loadFonts();

        /**
         * @brief Load saved user settings.
         */
        void loadUserSettings();

        /**
         * @brief Save user settings to disk.
         */
        void saveUserSettings();

        /**
         * @brief Perform post-guess processes.
         *
         * @param guessResult The result of the associated guess for the active game.
         */
        void initiatePostGuess(WordleEngine::GuessResult guessResult);

        /**
         * @brief Handles play again input from the statistics modal.
         */
        void handlePlayGameInput();

        /**
         * @brief Handles letter input from the keyboard or key events.
         *
         * @param letter The input letter.
         */
        void handleLetterInput(QChar letter);

        /**
         * @brief Handles commit input from the keyboard or key events.
         */
        void handleCommitInput();

        /**
         * @brief Handles delete input from the keyboard or key events.
         */
        void handleDeleteInput();
};
