/**
 * @file WordleApp.cpp
 * @brief Wordle Application Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the WordleApp class, which
 * manages the overall application state and UI components.
 */


#include "ViewModel/WordleApp.hpp"


/**
 * @brief Handle logging for debugging throughout the application.
 *
 * @param type The type of message being logged (Debug, Warning, Critical, Fatal, Info).
 * @param context The context information including file, line, and function where the message originated.
 * @param message The message text to be logged.
 */
void debugLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &message) {
    // Ensure the logs Directory Exists //
    QDir logDir("../logs");
    if (!logDir.exists()) logDir.mkpath(".");

    // Log the Message //
    QFile outFile("../logs/debug.log");
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream ts(&outFile);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        ts << "[" << timestamp << "] " << message << "\n";
        outFile.close();
    }
}


/**
 * @brief Constructs a WordleApp object.
 *
 * @param parent The parent QWidget.
 */
WordleApp::WordleApp(QObject *parent) : QObject(parent) {
    // App Configurations //
    qApp->installEventFilter(this);
    qInstallMessageHandler(debugLogHandler);

    // Load Application Assets //
    loadFonts();
    loadUserSettings();
    statistics.loadStats(userSettings.statistics);

    // Construct Window //
    ui = new WordleUI();
    QScreen* display = QGuiApplication::primaryScreen();
    QRect displayGeometry = display->geometry();
    if (userSettings.windowSize.width() < 625) userSettings.windowSize.setWidth(625);
    else if (userSettings.windowSize.width() > displayGeometry.width()) userSettings.windowSize.setWidth(displayGeometry.width());
    if (userSettings.windowSize.height() < 800) userSettings.windowSize.setHeight(800);
    else if (userSettings.windowSize.height() > displayGeometry.height()) userSettings.windowSize.setWidth(displayGeometry.height());
    if (userSettings.windowPosition.x() < 0 || userSettings.windowPosition.x() > displayGeometry.width()) userSettings.windowPosition.setX((displayGeometry.width() - userSettings.windowSize.width()) / 2);
    if (userSettings.windowPosition.y() < 0 || userSettings.windowPosition.y() > displayGeometry.height()) userSettings.windowPosition.setY((displayGeometry.height() - userSettings.windowSize.height()) / 2);
    ui->setGeometry(userSettings.windowPosition.x(), userSettings.windowPosition.y(), userSettings.windowSize.width(), userSettings.windowSize.height());
    ui->setWindowTitle("Wordle");
    ui->setWindowIcon(QIcon("../assets/textures/window-icon-opaque.png"));

    // Connect UI Interaction Signals //
    connect(ui, &WordleUI::playGameInput,                       this, &WordleApp::handlePlayGameInput);
    connect(ui, &WordleUI::playAgainInput,                      this, &WordleApp::handlePlayGameInput);
    connect(ui, &WordleUI::keyboardLetterInput,                 this, &WordleApp::handleLetterInput);
    connect(ui, &WordleUI::keyboardEnterInput,                  this, &WordleApp::handleCommitInput);
    connect(ui, &WordleUI::keyboardBackspaceInput,              this, &WordleApp::handleDeleteInput);
    connect(ui, &WordleUI::openStatisticsInput,                 this, [this]() { ui->openStatistics(); acceptingGameInput = false; });
    connect(ui, &WordleUI::openHowToPlayInput,                  this, [this]() { ui->openHowToPlay(); acceptingGameInput = false; });
    connect(ui, &WordleUI::openSettingsInput,                   this, [this]() { ui->openSettings(); acceptingGameInput = false; });
    connect(ui, &WordleUI::closeStatisticsInput,                this, [this]() { ui->closeStatistics(); acceptingGameInput = true; });
    connect(ui, &WordleUI::closeHowToPlayInput,                 this, [this]() { ui->closeHowToPlay(); acceptingGameInput = true; });
    connect(ui, &WordleUI::closeSettingsInput,                  this, [this]() { ui->closeSettings(); acceptingGameInput = true; });
    connect(ui, &WordleUI::enableHardMode,                      this, [this]() { engine.setDifficulty(WordleEngine::Difficulty::HARD_MODE); userSettings.hardMode = true; });
    connect(ui, &WordleUI::enableDarkTheme,                     this, [this]() { ColorTheme::instance().setTheme(ColorTheme::Themes::DARK); userSettings.darkTheme = true; });
    connect(ui, &WordleUI::enableHighContrastMode,              this, [this]() { ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::HIGH_CONTRAST); userSettings.highContrastMode = true; });
    connect(ui, &WordleUI::enableOnscreenKeyboardInputOnly,     this, [this]() { onScreenKeyboardInputOnly = true; userSettings.onScreenKeyboardInputOnly = true; });
    connect(ui, &WordleUI::disableHardMode,                     this, [this]() { engine.setDifficulty(WordleEngine::Difficulty::NORMAL_MODE); userSettings.hardMode = false; });
    connect(ui, &WordleUI::disableDarkTheme,                    this, [this]() { ColorTheme::instance().setTheme(ColorTheme::Themes::LIGHT); userSettings.darkTheme = false; });
    connect(ui, &WordleUI::disableHighContrastMode,             this, [this]() { ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::REGULAR_CONTRAST); userSettings.highContrastMode = false; });
    connect(ui, &WordleUI::disableOnscreenKeyboardInputOnly,    this, [this]() { onScreenKeyboardInputOnly = false; userSettings.onScreenKeyboardInputOnly = false; });
    connect(ui, &WordleUI::disabledSwitchPressed,               this, [this]() { ui->addPopUpMessage("Hard mode can only be enabled at the start of a round"); });

    // Configure Default Start State for Application //
    if (userSettings.darkTheme) ColorTheme::instance().setTheme(ColorTheme::Themes::DARK);
    else if (!userSettings.darkTheme) ColorTheme::instance().setTheme(ColorTheme::Themes::LIGHT);
    if (userSettings.highContrastMode) ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::HIGH_CONTRAST);
    else if (!userSettings.highContrastMode) ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::REGULAR_CONTRAST);
    ui->setStatistics(statistics.getStats());
    ui->setHardModeSwitch(userSettings.hardMode);
    ui->setDarkThemeSwitch(userSettings.darkTheme);
    ui->setHighContrastModeSwitch(userSettings.highContrastMode);
    ui->setOnScreenKeyboardInputOnlySwitch(userSettings.onScreenKeyboardInputOnly);
    ui->show();
}


/**
 * @brief Event filter to capture application events
 */
bool WordleApp::eventFilter(QObject* obj, QEvent* event) {
    // Handle Key Press Event Inputs //
    if (event->type() == QEvent::KeyPress) {
        if (obj == ui) {
            if (!onScreenKeyboardInputOnly) {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                // Ignore Key Auto-Repeat //
                if (keyEvent->isAutoRepeat())
                return true;
                // Letters A-Z //
                if (keyEvent->key() >= Qt::Key_A && keyEvent->key() <= Qt::Key_Z) {
                    QChar letter = QChar('A' + (keyEvent->key() - Qt::Key_A));
                    handleLetterInput(letter);
                    return true;
                }
                // Enter / Return //
                if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                    handleCommitInput();
                    return true;
                }
                // Backspace //
                if (keyEvent->key() == Qt::Key_Backspace) {
                    handleDeleteInput();
                    return true;
                }
            }
        }
    }

    // Handle Window Resize Events //
    else if (event->type() == QEvent::Resize) {
        if (obj == ui) {
            QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
            userSettings.windowSize.setWidth(resizeEvent->size().width());
            userSettings.windowSize.setHeight(resizeEvent->size().height());
        }
    }

    // Handle Window Move Events //
    else if (event->type() == QEvent::Move) {
        if (obj == ui) {
            QMoveEvent* moveEvent = static_cast<QMoveEvent*>(event);
            userSettings.windowPosition.setX(moveEvent->pos().x());
            userSettings.windowPosition.setY(moveEvent->pos().y());
        }
    }

    // Handle Window Resize Events //
    else if (event->type() == QEvent::Close) {
        if (obj == ui) {
            QCloseEvent* closeEvent = static_cast<QCloseEvent*>(event);
            saveUserSettings();
        }
    }

    // Pass On Base Implementation //
    return QObject::eventFilter(obj, event);
}


/**
 * @brief Load fonts for the application UI to use.
 */
void WordleApp::loadFonts() {
    int fontID;
    QString fontFamily;
    std::vector<QString> fontPaths = {
        "../assets/fonts/NYTKarnak-Condensed-Bold.ttf",
        "../assets/fonts/NYTStymie-Regular-500.ttf",
        "../assets/fonts/HelveticaNeue-Regular-Light.otf",
        "../assets/fonts/HelveticaNeue-Regular-Normal.otf",
        "../assets/fonts/HelveticaNeue-Regular-Medium.otf",
        "../assets/fonts/HelveticaNeue-Regular-Bold.otf",
        "../assets/fonts/NYTFranklin-Regular-700.ttf",
        "../assets/fonts/NYTFranklin-Regular-800.ttf",
        "../assets/fonts/Arial-Regular-Normal.TTF",
        "../assets/fonts/Arial-Regular-Medium.TTF",
        "../assets/fonts/Arial-Regular-Bold.TTF",
        "../assets/fonts/FontIcons.ttf"
    };

    for (const auto& fontPath : fontPaths) {
        fontID = QFontDatabase::addApplicationFont(fontPath);
        if (fontID == -1) qWarning() << "Failed to Load Font" << fontPath;
        fontFamily = QFontDatabase::applicationFontFamilies(fontID).at(0);
    }
}


/**
 * @brief Load saved user settings.
 */
void WordleApp::loadUserSettings() {
    QString userSettingsFilePath = "../assets/settings/settings.json";
    QFile userSettingsFile(userSettingsFilePath);

    if (!userSettingsFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open settings file for reading:" << userSettingsFilePath;
        return;
    }
    QByteArray data = userSettingsFile.readAll();
    userSettingsFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return;
    }
    QJsonObject json = doc.object();

    // Window Size //
    if (json.contains("window-size") && json["window-size"].isArray()) {
        QJsonArray sizeArray = json["window-size"].toArray();
        if (sizeArray.size() == 2) {
            userSettings.windowSize = QSize(sizeArray[0].toInt(), sizeArray[1].toInt());
        }
    }

    // Window Position //
    if (json.contains("window-position") && !json["window-position"].isNull() && json["window-position"].isArray()) {
        QJsonArray posArray = json["window-position"].toArray();
        if (posArray.size() == 2) {
            userSettings.windowPosition = QPoint(posArray[0].toInt(), posArray[1].toInt());
        }
    }   else {
        userSettings.windowPosition = QPoint(-1, -1);
    }

    // Boolean Settings //
    userSettings.hardMode = json["hard-mode"].toBool(false);
    userSettings.darkTheme = json["dark-theme"].toBool(true);
    userSettings.highContrastMode = json["high-contrast-mode"].toBool(false);
    userSettings.onScreenKeyboardInputOnly = json["onscreen-keyboard-input-only"].toBool(false);

    // Statistics Settings //
    if (json.contains("statistics") && json["statistics"].isObject()) {
        QJsonObject stats = json["statistics"].toObject();
        userSettings.statistics.gamesPlayed = stats["games-played"].toInt(0);
        userSettings.statistics.winPercentage = stats["win-percentage"].toInt(0);
        userSettings.statistics.currentStreak = stats["current-streak"].toInt(0);
        userSettings.statistics.maxStreak = stats["max-streak"].toInt(0);
        if (stats.contains("guess-distribution") && stats["guess-distribution"].isArray()) {
            QJsonArray distArray = stats["guess-distribution"].toArray();
            for (int i = 0; i < userSettings.statistics.guessDistribution.size(); i++) {
                userSettings.statistics.guessDistribution[i] = (i <= distArray.size() - 1) ? distArray[i].toInt(0) : 0;
            }
        }
    }
}


/**
 * @brief Save user settings to disk.
 */
void WordleApp::saveUserSettings() {
    QJsonObject json;
    QString userSettingsFilePath = "../assets/settings/settings.json";

    // Window Size //
    QJsonArray windowSizeArray;
    windowSizeArray.append(userSettings.windowSize.width());
    windowSizeArray.append(userSettings.windowSize.height());
    json["window-size"] = windowSizeArray;

    // Window Position (Null if Invalid) //
    if (userSettings.windowPosition.isNull()) {
        json["window-position"] = QJsonValue::Null;
    }   else {
        QJsonArray windowPosArray;
        windowPosArray.append(userSettings.windowPosition.x());
        windowPosArray.append(userSettings.windowPosition.y());
        json["window-position"] = windowPosArray;
    }

    // Boolean Settings //
    json["hard-mode"] = userSettings.hardMode;
    json["dark-theme"] = userSettings.darkTheme;
    json["high-contrast-mode"] = userSettings.highContrastMode;
    json["onscreen-keyboard-input-only"] = userSettings.onScreenKeyboardInputOnly;

    // Statistics Settings //
    QJsonObject statsObj;
    statsObj["games-played"] = static_cast<int>(userSettings.statistics.gamesPlayed);
    statsObj["win-percentage"] = static_cast<int>(userSettings.statistics.winPercentage);
    statsObj["current-streak"] = static_cast<int>(userSettings.statistics.currentStreak);
    statsObj["max-streak"] = static_cast<int>(userSettings.statistics.maxStreak);

    QJsonArray guessDistArray;
    for (int val : userSettings.statistics.guessDistribution) {
        guessDistArray.append(val);
    }
    statsObj["guess-distribution"] = guessDistArray;
    json["statistics"] = statsObj;

    // Create Directory if it Doesn't Exist //
    QFileInfo fileInfo(userSettingsFilePath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Write to User Settings to File //
    QFile file(userSettingsFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Could not open settings file for writing:" << userSettingsFilePath;
        return;
    }
    QJsonDocument doc(json);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}


/**
 * @brief Perform post-guess processes.
 *
 * @param guessResult The result of the associated guess for the active game.
 */
void WordleApp::initiatePostGuess(WordleEngine::GuessResult guessResult) {
    // Set the Visual State of the Keyboard //
    for (int i = 0; i < guessResult.guess.size(); i++) {
        ui->setKeyboardKeyState(QChar(guessResult.guess[i]), guessResult.letterStates[i]);
    }

    // Handle Game Won or Lost Logic //
    if (guessResult.guessValidity == WordleEngine::GuessValidity::GuessIncorrect) {
        acceptingGameInput = true;
    }
    if (guessResult.guessValidity == WordleEngine::GuessValidity::GameWon || guessResult.guessValidity == WordleEngine::GuessValidity::GameLost) {
        acceptingGameInput = false;

        // Add the Pop Up Message //
        ui->addPopUpMessage(QString::fromStdString(guessResult.message));

        // Determine Game Outcome //
        bool gameWon;
        if (guessResult.guessValidity == WordleEngine::GuessValidity::GameLost) gameWon = false;
        else if (guessResult.guessValidity == WordleEngine::GuessValidity::GameWon) gameWon = true;

        // Track Game Stats, Cache Stats in User Settings, Upload Stats to Statistics Modal //
        statistics.addGameStats(gameWon, guessResult.numAttempts);
        WordleStatistics::Statistics newStats = statistics.getStats();
        userSettings.statistics = newStats;
        ui->setStatistics(newStats);
        ui->enableRestart();

        // Open Statistics Screen if Game Won //
        if (gameWon) QTimer::singleShot(2000, this, [this, guessResult]() { ui->openStatistics(); });
    }
}


/**
 * @brief Handles play again input from the statistics modal.
 */
void WordleApp::handlePlayGameInput() {
    engine.startNewGame();
    ui->enableHardModeToggling();
    ui->disableRestart();
    ui->resetGameUI();
    ui->closeIntroScreen();
    ui->closeHowToPlay();
    ui->closeStatistics();
    ui->closeSettings();
    ui->resetGameUI();
    acceptingGameInput = true;
}


/**
 * @brief Handles letter input from the keyboard or key events.
 *
 * @param letter The input letter.
 */
void WordleApp::handleLetterInput(QChar letter) {
    if (!acceptingGameInput) return;

    if (!engine.getIsActive()) return;
    if (currentGuess.length() >= GameConfig::WORD_LENGTH) return;
    currentGuess += QString(letter).toStdString();
    ui->appendLetter(letter);
}


/**
 * @brief Handles commit input from the keyboard or key events.
 */
void WordleApp::handleCommitInput() {
    if (!acceptingGameInput) return;

    WordleEngine::GuessResult guessResult = engine.makeGuess(currentGuess);

    // Game is Not Active //
    if (guessResult.guessValidity == WordleEngine::GuessValidity::GameInactive) {
        currentGuess = "";
    }

    // Guess Word Too Short, Too Long, Not a Word, or Not Adhered to Previous Correct or Present Letter //
    else if (guessResult.guessValidity == WordleEngine::GuessValidity::GuessTooShort ||
             guessResult.guessValidity == WordleEngine::GuessValidity::GuessTooLong ||
             guessResult.guessValidity == WordleEngine::GuessValidity::GuessWordInvalid ||
             guessResult.guessValidity == WordleEngine::GuessValidity::PreviousCorrectLetterWordInvalid ||
             guessResult.guessValidity == WordleEngine::GuessValidity::PreviousPresentLetterWordInvalid
    ) {
        ui->addPopUpMessage(QString::fromStdString(guessResult.message));
        ui->invalidGuess();
    }

    // Guess Word Incorrect, Guess Word Correct (Game Won), or Guess Word Incorrect and Out of Turns (Game Lost) //
    else if (guessResult.guessValidity == WordleEngine::GuessValidity::GuessIncorrect ||
             guessResult.guessValidity == WordleEngine::GuessValidity::GameWon ||
             guessResult.guessValidity == WordleEngine::GuessValidity::GameLost
    ) {
        currentGuess = "";
        acceptingGameInput = false;
        ui->commitGuess(guessResult.letterStates);
        if (guessResult.numAttempts >= 1 && engine.getDifficulty() == WordleEngine::Difficulty::NORMAL_MODE) ui->disableHardModeToggling();
        connect(ui, &WordleUI::commitLettersAnimationFinished, this, [this, guessResult]() { initiatePostGuess(guessResult); }, Qt::SingleShotConnection);
    }
}


/**
 * @brief Handles delete input from the keyboard or key events.
 */
void WordleApp::handleDeleteInput() {
    if (!acceptingGameInput) return;

    if (currentGuess.empty()) return;
    currentGuess.pop_back();
    ui->deleteLetter();
}
