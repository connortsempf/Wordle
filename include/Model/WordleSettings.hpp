/**
 * @file WordleSettings.cpp
 * @brief Wordle Settings Management Definition.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the definition of the _WordleSettings class, which
 * manages the settings state and persistence for the application.
 */


#pragma once
#include <QScreen>
#include <QSize>
#include <QPoint>
#include <QString>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <array>
#include <algorithm>
#include "Model/GameConfig.hpp"


/**
 * @brief The blueprint for the Wordle application settings manager class.
 */
class WordleSettings {

    public:
        /**
         * @brief Statistics struct that contains all necessary stats data for wordle games.
         */
        struct Statistics {
            /**
             * @brief The number of total games played.
             */
            unsigned int gamesPlayed = 0;

            /**
             * @brief The percentage of games won as a [0, 100] int (not a [0, 1] float).
             */
            unsigned int winPercentage = 0;

            /**
             * @brief The current consecutive win streak.
             */
            unsigned int currentStreak = 0;

            /**
             * @brief The maximum consecutive win streak ever.
             */
            unsigned int maxStreak = 0;

            /**
             * @brief The total number of games won for each number of guesses.
             */
            std::array<unsigned int, GameConfig::NUM_GUESSES> guessDistribution = { 0, 0, 0, 0, 0, 0 };
        };

        /**
         * @brief Settings data structure for persistent app configuration.
         */
        struct Settings {

            /**
             * @brief Window is maximized setting.
             */
            bool isWindowMaximized = false;

            /**
             * @brief Window size setting.
             */
            QSize windowSize = {625, 800};

            /**
             * @brief On-screen window position setting.
             */
            QPoint windowPosition = {-1, -1};

            /**
             * @brief Hard mode difficulty setting.
             */
            bool hardMode = false;

            /**
             * @brief Dark theme setting.
             */
            bool darkTheme = true;

            /**
             * @brief high-contrast mode setting.
             */
            bool highContrastMode = false;

            /**
             * @brief On-screen keyboard input only setting.
             */
            bool onScreenKeyboardInputOnly = false;

            /**
             * @brief Wordle statistics object settings.
             */
            WordleSettings::Statistics statistics;
        };

    public:
        /**
         * @brief Default constructor - initializes settings with defaults.
         */
        WordleSettings() = default;

        /**
         * @brief Load settings from disk (or create with defaults if doesn't exist).
         */
        void loadSettings();

        /**
         * @brief Save current settings to disk.
         */
        void saveSettings();

        /**
         * @brief Add the results of a wordle game to the current statistics.
         *
         * @param gameWon The win/lose status of the latest played game.
         * @param numGuesses The number of guesses used by the player in the latest played game.
         */
        void updateGameStats(bool gameWon, unsigned int numGuesses);

        /**
         * @brief Get current settings.
         *
         * @return Reference to the settings struct so it can be updated.
         */
        const Settings& getSettings() const;

        /**
         * @brief Get default settings.
         *
         * @return Reference to the defualt settings struct.
         */
        const Settings& getDefaultSettings() const;

        /**
         * @brief The settings file path.
         */
        const QString getSettingsFilePath() const;

        /**
         * @brief Set the window is maximized setting.
         *
         * @param isWindowMaximized True if the window is maximized, false if otherwise.
         */
        void setIsWindowMaximized(bool isWindowMaximized);

        /**
         * @brief Set the window size setting.
         *
         * @param newWindowSize The new on-screen window size.
         */
        void setWindowSize(QSize newWindowSize);

        /**
         * @brief Set the on-screen window position setting.
         *
         * @param newWindowPosition The new on-screen window position.
         */
        void setWindowPosition(QPoint newWindowPosition);

        /**
         * @brief Set the hard mode difficulty setting.
         *
         * @param isHardMode True if the game is in hard mode, false if otherwise.
         */
        void setHardMode(bool isHardMode);

        /**
         * @brief Set the dark theme setting.
         *
         * @param isDarkTheme True if the dark theme is active, false if otherwise.
         */
        void setDarkTheme(bool isDarkTheme);

        /**
         * @brief set the high-contrast mode setting.
         *
         * @param isHighContrastMode True if the high contrast mode is active, false if otherwise.
         */
        void setHighContrastMode(bool isHighContrastMode);

        /**
         * @brief Set the on-screen keyboard input only setting.
         *
         * @param isOnScreenKeyboardInputOnly True if the on-screen keyboard input only is active, false if otherwise.
         */
        void setOnScreenKeyboardInputOnly(bool isOnScreenKeyboardInputOnly);

    private:
        /**
         * @brief The resource file path for the default settings file.
         */
        static const QString defaultSettingsPath;

        /**
         * @brief The file name suffix for the config settings file.
         */
        static const QString configSettingsFile;

    private:
        /**
         * @brief The settings container.
         */
        Settings settings;

        /**
         * @brief The default settings container.
         */
        const Settings defaultSettings;
};
