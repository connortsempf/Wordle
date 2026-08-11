/**
 * @file WordleSettings.hpp
 * @brief Wordle Settings Management Implementation.
 * @date 2026-01-25
 * @version 1.0.0
 *
 * This file contains the implementation of the WordleSettings class, which
 * manages the settings state and persistence for the application.
 */


#include "Model/WordleSettings.hpp"


/**
 * @brief The resource file path for the default settings file.
 */
const QString WordleSettings::defaultSettingsPath = ":/config/default-settings.json";

/**
 * @brief The file name suffix for the config settings file.
 */
const QString WordleSettings::configSettingsFile = "/settings.json";


/**
 * @brief Load settings from disk (or create with defaults if doesn't exist).
 */
void WordleSettings::loadSettings() {
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir configDir(configPath);

    // Ensure Directory Exists //
    if (!configDir.exists()) {
        configDir.mkpath(".");
    }

    QString settingsFilePath = getSettingsFilePath();
    QFile settingsFile(settingsFilePath);

    // If Settings Don't Exist, Just use the Default Settings Struct  //
    if (!settingsFile.exists()) {
        setWindowSize(defaultSettings.windowSize);
        setWindowPosition(defaultSettings.windowPosition);
        saveSettings();
        return;
    }

    // Open and Read Settings //
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = settingsFile.readAll();
    settingsFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        return;
    }

    if (!doc.isObject()) {
        return;
    }


    QJsonObject json = doc.object();

    // Window Maximized Setting //
    if (json.contains("is-window-maximized")) {
        settings.isWindowMaximized = json["is-window-maximized"].toBool();
    }

    // Window Size Setting //
    if (json.contains("window-size") && json["window-size"].isArray()) {
        QJsonArray sizeArray = json["window-size"].toArray();
        if (sizeArray.size() == 2) {
            setWindowSize(QSize(sizeArray[0].toInt(), sizeArray[1].toInt()));
        }
    }

    // Window Position Setting //
    if (json.contains("window-position") && json["window-position"].isArray()) {
        QJsonArray posArray = json["window-position"].toArray();
        if (posArray.size() == 2) {
            setWindowPosition(QPoint(posArray[0].toInt(), posArray[1].toInt()));
        }
    }   else {
        setWindowPosition(defaultSettings.windowPosition);
    }

    // Hard Mode Setting //
    if (json.contains("hard-mode")) {
        settings.hardMode = json["hard-mode"].toBool();
    }

    // Dark Theme Setting //
    if (json.contains("dark-theme")) {
        settings.darkTheme = json["dark-theme"].toBool();
    }

    // High-Contrast Mode Setting //
    if (json.contains("high-contrast-mode")) {
        settings.highContrastMode = json["high-contrast-mode"].toBool();
    }

    // On-Screen Keyboard Input Only Setting //
    if (json.contains("on-screen-keyboard-input-only")) {
        settings.onScreenKeyboardInputOnly = json["on-screen-keyboard-input-only"].toBool();
    }

    // Statistics Settings //
    if (json.contains("statistics") && json["statistics"].isObject()) {
        QJsonObject statsJSON = json["statistics"].toObject();
        settings.statistics.gamesPlayed = statsJSON["games-played"].toInt();
        settings.statistics.winPercentage = statsJSON["win-percentage"].toInt();
        settings.statistics.currentStreak = statsJSON["current-streak"].toInt();
        settings.statistics.maxStreak = statsJSON["max-streak"].toInt();
        if (statsJSON.contains("guess-distribution") && statsJSON["guess-distribution"].isArray()) {
            QJsonArray distributionJSON = statsJSON["guess-distribution"].toArray();
            for (int i = 0; i < distributionJSON.size(); i++) {
                settings.statistics.guessDistribution[i] = distributionJSON[i].toInt();
            }
        }
    }
}


/**
 * @brief Save current settings to disk.
 */
void WordleSettings::saveSettings() {
    QString settingsFilePath = getSettingsFilePath();

    // Construct Settings //
    QJsonObject json;
    json["is-window-maximized"] = settings.isWindowMaximized;
    json["window-size"] = QJsonArray{settings.windowSize.width(), settings.windowSize.height()};
    json["window-position"] = QJsonArray{settings.windowPosition.x(), settings.windowPosition.y()};
    json["hard-mode"] = settings.hardMode;
    json["dark-theme"] = settings.darkTheme;
    json["high-contrast-mode"] = settings.highContrastMode;
    json["on-screen-keyboard-input-only"] = settings.onScreenKeyboardInputOnly;

    QJsonArray guessDistribution;
    for (int value : settings.statistics.guessDistribution) {
        guessDistribution.append(value);
    }

    json["statistics"] = QJsonObject{
        {"games-played", static_cast<int>(settings.statistics.gamesPlayed)},
        {"win-percentage", static_cast<int>(settings.statistics.winPercentage)},
        {"current-streak", static_cast<int>(settings.statistics.currentStreak)},
        {"max-streak", static_cast<int>(settings.statistics.maxStreak)},
        {"guess-distribution", guessDistribution}
    };

    QJsonDocument doc(json);
    QFile file(settingsFilePath);

    // Write to Disk //
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}


/**
 * @brief Add the results of a wordle game to the current statistics.
 *
 * @param gameWon The win/lose status of the latest played game.
 * @param numGuesses The number of guesses used by the player in the latest played game.
 */
void WordleSettings::updateGameStats(bool gameWon, unsigned int numGuesses) {
    unsigned int oldGamesPlayed = settings.statistics.gamesPlayed;
    unsigned int oldGamesWon = 0;
    for (const auto& distributedWins : settings.statistics.guessDistribution) oldGamesWon += distributedWins;

    ++settings.statistics.gamesPlayed;
    if (gameWon) {
        settings.statistics.winPercentage = static_cast<unsigned int>((static_cast<float>(++oldGamesWon) / settings.statistics.gamesPlayed) * 100);
        ++settings.statistics.currentStreak;
        settings.statistics.maxStreak = settings.statistics.currentStreak > settings.statistics.maxStreak ? settings.statistics.currentStreak : settings.statistics.maxStreak;
        if (numGuesses > 0 && numGuesses < 7) ++(settings.statistics.guessDistribution[numGuesses - 1]);
    }   else if (!gameWon) {
        settings.statistics.winPercentage = static_cast<unsigned int>((static_cast<float>(oldGamesWon) / settings.statistics.gamesPlayed) * 100);
        settings.statistics.currentStreak = 0;
    }
}


/**
 * @brief Get current settings.
 *
 * @return Reference to the settings struct so it can be updated.
 */
const WordleSettings::Settings& WordleSettings::getSettings() const {
    return settings;
}


/**
 * @brief Get default settings.
 *
 * @return Reference to the defualt settings struct.
 */
const WordleSettings::Settings& WordleSettings::getDefaultSettings() const {
    return defaultSettings;
}


/**
 * @brief Get the settings file path.
 */
const QString WordleSettings::getSettingsFilePath() const {
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + WordleSettings::configSettingsFile;
}


/**
 * @brief Set the window is maximized setting.
 *
 * @param isWindowMaximized True if the window is maximized, false if otherwise.
 */
void WordleSettings::setIsWindowMaximized(bool isWindowMaximized) {
    settings.isWindowMaximized = isWindowMaximized;
}


/**
 * @brief Set the window size setting.
 *
 * @param newWindowSize The new on-screen window size.
 */
void WordleSettings::setWindowSize(QSize newWindowSize) {
    if (!settings.isWindowMaximized) {
        QScreen* display = QGuiApplication::primaryScreen();
        QRect displayGeometry = display->geometry();
        settings.windowSize.setWidth(std::clamp(newWindowSize.width(), defaultSettings.windowSize.width(), displayGeometry.width()));
        settings.windowSize.setHeight(std::clamp(newWindowSize.height(), defaultSettings.windowSize.height(), displayGeometry.height()));
    }
}


/**
 * @brief Set the on-screen window position setting.
 *
 * @param newWindowPosition The new on-screen window position.
 */
void WordleSettings::setWindowPosition(QPoint newWindowPosition) {
    if (!settings.isWindowMaximized) {
        // Get Display and Settings Information //
        QScreen* display = QGuiApplication::primaryScreen();
        QRect displayGeometry = display->geometry();
        settings.windowPosition = newWindowPosition;

        // Is Window Completely Off-Screen //
        if (!displayGeometry.intersects(QRect(newWindowPosition, settings.windowSize))) {
            settings.windowPosition.setX((displayGeometry.width() - settings.windowSize.width()) / 2);
            settings.windowPosition.setY((displayGeometry.height() - settings.windowSize.height()) / 2);
        }
        // Ensure Window is Fully Visible and Not Cut Off at Edges //
        else {
            if (newWindowPosition.x() < displayGeometry.x() || newWindowPosition.x() + settings.windowSize.width() > displayGeometry.right()) {
                settings.windowPosition.setX((displayGeometry.width() - settings.windowSize.width()) / 2);
            }
            if (newWindowPosition.y() < displayGeometry.y() || newWindowPosition.y() + settings.windowSize.height() > displayGeometry.bottom()) {
                settings.windowPosition.setY((displayGeometry.height() - settings.windowSize.height()) / 2);
            }
        }
    }
}


/**
 * @brief Set the hard mode difficulty setting.
 *
 * @param isHardMode True if the game is in hard mode, false if otherwise.
 */
void WordleSettings::setHardMode(bool isHardMode) {
    settings.hardMode = isHardMode;
}


/**
 * @brief Set the dark theme setting.
 *
 * @param isDarkTheme True if the dark theme is active, false if otherwise.
 */
void WordleSettings::setDarkTheme(bool isDarkTheme) {
    settings.darkTheme = isDarkTheme;
}


/**
 * @brief set the high-contrast mode setting.
 *
 * @param isHighContrastMode True if the high contrast mode is active, false if otherwise.
 */
void WordleSettings::setHighContrastMode(bool isHighContrastMode) {
    settings.highContrastMode = isHighContrastMode;
}


/**
 * @brief Set the on-screen keyboard input only setting.
 *
 * @param isOnScreenKeyboardInputOnly True if the on-screen keyboard input only is active, false if otherwise.
 */
void WordleSettings::setOnScreenKeyboardInputOnly(bool isOnScreenKeyboardInputOnly) {
    settings.onScreenKeyboardInputOnly = isOnScreenKeyboardInputOnly;
}
