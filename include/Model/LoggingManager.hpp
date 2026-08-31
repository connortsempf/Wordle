/**
 * @file LoggingManager.hpp
 * @brief Logging Management for Wordle.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains definition for the logging management
 * of the Wordle Application.
 */


#pragma once
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>


/**
 * @brief The logging managing class for the Wordle game.
 */
class LoggingManager {

    public:
        /**
         * @brief Explicit constructor of a Log Manager object.
         */
        explicit LoggingManager();

        /**
         * @brief Deleted Copy Constructor.
         */
        LoggingManager(const LoggingManager&) = delete;

        /**
         * @brief Deleted Copy Assignment Constructor.
         */
        LoggingManager& operator=(const LoggingManager&) = delete;

        /**
         * @brief Deleted Move Constructor.
         */
        LoggingManager(LoggingManager&&) = delete;

        /**
         * @brief Deleted Move Assignment Constructor.
         */
        LoggingManager& operator=(LoggingManager&&) = delete;

        /**
         * @brief The debug mode configuration logging file path.
         */
        const QString& getDebugModeLoggingFilePath() const;

        /**
         * @brief The releasedebug mode configuration logging file path.
         */
        const QString& getReleaseModeLoggingFilePath() const;

        /**
         * @brief Handle application logs.
         *
         * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
         * @param context The context information including file, line, and function where the message originated.
         * @param message The message text to be logged.
         */
        static void messageLoggingHandler(QtMsgType type, const QMessageLogContext& context, const QString& message);

    private:
        /**
         * @brief The title for the logging directory.
         */
        const QString loggingDirectoryTitle;

        /**
         * @brief The logging path for debug mode configuration logging location.
         */
        const QString debugModeLoggingDirectoryPath;

        /**
         * @brief The logging path for release mode configuration logging location.
         */
        const QString releaseModeLoggingDirectoryPath;

        /**
         * @brief The file name of logging file.
         */
        const QString loggingFileName;

        /**
         * @brief The full logging file path for debug mode configuration logging file.
         */
        const QString debugModeLoggingFilePath;

        /**
         * @brief The full logging file path for debug mode configuration logging file.
         */
        const QString releaseModeLoggingFilePath;

        /**
         * @brief The active configuration's logging file path.
         */
        static QString activeConfigurationModeLoggingFilePath;
};
