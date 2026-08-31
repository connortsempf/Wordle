/**
 * @file LoggingManager.cpp
 * @brief Logging Management for Wordle.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains implementation for the logging
 * management of the Wordle Application.
 */


#include "Model/LoggingManager.hpp"


/**
 * @brief The active configuration's logging file path.
 */
QString LoggingManager::activeConfigurationModeLoggingFilePath = "";


/**
 * @brief Explicit constructor of a Log Manager object.
 */
LoggingManager::LoggingManager() :
    loggingDirectoryTitle("/logs"),
    debugModeLoggingDirectoryPath(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(".." + loggingDirectoryTitle)),
    releaseModeLoggingDirectoryPath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + LoggingManager::loggingDirectoryTitle),
    loggingFileName("Wordle.log"),
    debugModeLoggingFilePath(debugModeLoggingDirectoryPath + "/" + loggingFileName),
    releaseModeLoggingFilePath(releaseModeLoggingDirectoryPath + "/" + loggingFileName) {

    #ifdef DEBUG
        LoggingManager::activeConfigurationModeLoggingFilePath = debugModeLoggingFilePath;
    #elif RELEASE
        LoggingManager::activeConfigurationModeLoggingFilePath = releaseModeLoggingFilePath;
    #else
        #error "No build configuration defined: expected DEBUG or RELEASE"
    #endif

    QDir loggingDir(QFileInfo(activeConfigurationModeLoggingFilePath).absolutePath());
    if (!loggingDir.exists()) {
        loggingDir.mkpath(".");
    }
}


/**
 * @brief The debug mode configuration logging file path.
 */
const QString& LoggingManager::getDebugModeLoggingFilePath() const {
    return debugModeLoggingFilePath;
}


/**
 * @brief The releasedebug mode configuration logging file path.
 */
const QString& LoggingManager::getReleaseModeLoggingFilePath() const {
    return releaseModeLoggingFilePath;
}


/**
 * @brief Handle application logs.
 *
 * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
 * @param context The context information including file, line, and function where the message originated.
 * @param message The message text to be logged.
 */
void LoggingManager::messageLoggingHandler(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    QFile outFile(activeConfigurationModeLoggingFilePath);

    QString logType;
    switch (type) {
        case QtDebugMsg:
            logType = "DEBUG";
            break;
        case QtInfoMsg:
            logType = "INFO";
            break;
        case QtWarningMsg:
            logType = "WARNING";
            break;
        case QtCriticalMsg:
            logType = "CRITICAL";
            break;
        case QtFatalMsg:
            logType = "FATAL";
            break;
        default:
            logType = "UNKNOWN";
    }

    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream outfileTextStream(&outFile);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        if (type == QtDebugMsg) {
            outfileTextStream << "[" << timestamp << "] | " << context.file << ":" << context.line << " | " << logType << " | " << message << "\n";
        }
        else {
            outfileTextStream << "[" << timestamp << "] | " << logType << " | " << message << "\n";
        }
        outFile.close();
    }
}
