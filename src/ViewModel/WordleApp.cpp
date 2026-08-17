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
    return;
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
    settingsManager.loadSettings();
    setupWindow();

    // Connect UI Interaction Signals //
    connect(uiManager, &WordleUI::playGameInput,                       this, &WordleApp::handlePlayGameInput);
    connect(uiManager, &WordleUI::playAgainInput,                      this, &WordleApp::handlePlayGameInput);
    connect(uiManager, &WordleUI::keyboardLetterInput,                 this, &WordleApp::handleLetterInput);
    connect(uiManager, &WordleUI::keyboardEnterInput,                  this, &WordleApp::handleCommitInput);
    connect(uiManager, &WordleUI::keyboardBackspaceInput,              this, &WordleApp::handleDeleteInput);
    connect(uiManager, &WordleUI::openStatisticsInput,                 this, [this]() { uiManager->openStatistics(); acceptingGameInput = false; });
    connect(uiManager, &WordleUI::openHowToPlayInput,                  this, [this]() { uiManager->openHowToPlay(); acceptingGameInput = false; });
    connect(uiManager, &WordleUI::openSettingsInput,                   this, [this]() { uiManager->openSettings(); acceptingGameInput = false; });
    connect(uiManager, &WordleUI::closeStatisticsInput,                this, [this]() { uiManager->closeStatistics(); acceptingGameInput = true; });
    connect(uiManager, &WordleUI::closeHowToPlayInput,                 this, [this]() { uiManager->closeHowToPlay(); acceptingGameInput = true; });
    connect(uiManager, &WordleUI::closeSettingsInput,                  this, [this]() { uiManager->closeSettings(); acceptingGameInput = true; });
    connect(uiManager, &WordleUI::enableHardMode,                      this, [this]() { engine.setDifficulty(WordleEngine::Difficulty::HARD_MODE); settingsManager.setHardMode(true); });
    connect(uiManager, &WordleUI::enableDarkTheme,                     this, [this]() { ColorTheme::instance().setTheme(ColorTheme::Themes::DARK); settingsManager.setDarkTheme(true); });
    connect(uiManager, &WordleUI::enableHighContrastMode,              this, [this]() { ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::HIGH_CONTRAST); settingsManager.setHighContrastMode(true); });
    connect(uiManager, &WordleUI::enableOnscreenKeyboardInputOnly,     this, [this]() { settingsManager.setOnScreenKeyboardInputOnly(true); });
    connect(uiManager, &WordleUI::disableHardMode,                     this, [this]() { engine.setDifficulty(WordleEngine::Difficulty::NORMAL_MODE); settingsManager.setHardMode(false); });
    connect(uiManager, &WordleUI::disableDarkTheme,                    this, [this]() { ColorTheme::instance().setTheme(ColorTheme::Themes::LIGHT); settingsManager.setDarkTheme(false); });
    connect(uiManager, &WordleUI::disableHighContrastMode,             this, [this]() { ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::REGULAR_CONTRAST); settingsManager.setHighContrastMode(false); });
    connect(uiManager, &WordleUI::disableOnscreenKeyboardInputOnly,    this, [this]() { settingsManager.setOnScreenKeyboardInputOnly(false); });
    connect(uiManager, &WordleUI::disabledSwitchPressed,               this, [this]() { uiManager->addPopUpMessage("Hard mode can only be enabled at the start of a round"); });

    // Configure Default Start State for Application //
    const WordleSettings::Settings& settings = settingsManager.getSettings();
    if (settings.darkTheme) {
        ColorTheme::instance().setTheme(ColorTheme::Themes::DARK);
    }   else {
        ColorTheme::instance().setTheme(ColorTheme::Themes::LIGHT);
    }
    if (settings.highContrastMode) {
        ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::HIGH_CONTRAST);
    }   else {
        ColorTheme::instance().setContrastMode(ColorTheme::ContrastMode::REGULAR_CONTRAST);
    }
    uiManager->setStatistics(settings.statistics);
    uiManager->setHardModeSwitch(settings.hardMode);
    uiManager->setDarkThemeSwitch(settings.darkTheme);
    uiManager->setHighContrastModeSwitch(settings.highContrastMode);
    uiManager->setOnScreenKeyboardInputOnlySwitch(settings.onScreenKeyboardInputOnly);
    uiManager->show();
}


/**
 * @brief Event filter to capture application events
 */
bool WordleApp::eventFilter(QObject* obj, QEvent* event) {
    // Handle Key Press Event Inputs //
    if (event->type() == QEvent::KeyPress) {
        if (obj == uiManager) {
            const WordleSettings::Settings& settings = settingsManager.getSettings();
            if (!settings.onScreenKeyboardInputOnly) {
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
        if (obj == uiManager) {
            QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
            settingsManager.setWindowSize(QSize(resizeEvent->size().width(), resizeEvent->size().height()));
        }
    }

    // Handle Window Move Events //
    else if (event->type() == QEvent::Move) {
        if (obj == uiManager) {
            QMoveEvent* moveEvent = static_cast<QMoveEvent*>(event);
            settingsManager.setWindowPosition(QPoint(moveEvent->pos().x(), moveEvent->pos().y()));
        }
    }

    // Handle Window Resize Events //
    else if (event->type() == QEvent::Close) {
        if (obj == uiManager) {
            QCloseEvent* closeEvent = static_cast<QCloseEvent*>(event);
            settingsManager.saveSettings();
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
    std::vector<QString> fontPaths = {
        ":/fonts/assets/fonts/NYTKarnak-Condensed-Bold.ttf",
        ":/fonts/assets/fonts/NYTStymie-Regular-500.ttf",
        ":/fonts/assets/fonts/HelveticaNeue-Regular-Light.otf",
        ":/fonts/assets/fonts/HelveticaNeue-Regular-Normal.otf",
        ":/fonts/assets/fonts/HelveticaNeue-Regular-Medium.otf",
        ":/fonts/assets/fonts/HelveticaNeue-Regular-Bold.otf",
        ":/fonts/assets/fonts/NYTFranklin-Regular-700.ttf",
        ":/fonts/assets/fonts/NYTFranklin-Regular-800.ttf",
        ":/fonts/assets/fonts/Arial-Regular-Normal.TTF",
        ":/fonts/assets/fonts/Arial-Regular-Medium.TTF",
        ":/fonts/assets/fonts/Arial-Regular-Bold.TTF",
        ":/fonts/assets/fonts/FontIcons.ttf"
    };

    for (const auto& fontPath : fontPaths) {
        fontID = QFontDatabase::addApplicationFont(fontPath);
        if (fontID == -1) {
            continue;
        }

        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontID);
        if (!fontFamilies.isEmpty()) {
            QString fontFamily = fontFamilies.at(0);
        }
    }
}


/**
 * @brief Setup the application window.
 */
void WordleApp::setupWindow() {
    uiManager = new WordleUI();
    const WordleSettings::Settings& settings = settingsManager.getSettings();
    uiManager->setGeometry(settings.windowPosition.x(), settings.windowPosition.y(), settings.windowSize.width(), settings.windowSize.height());
    uiManager->setWindowTitle("Wordle");
}


/**
 * @brief Perform post-guess processes.
 *
 * @param guessResult The result of the associated guess for the active game.
 */
void WordleApp::initiatePostGuess(WordleEngine::GuessResult guessResult) {
    // Set the Visual State of the Keyboard //
    for (int i = 0; i < guessResult.guess.size(); i++) {
        uiManager->setKeyboardKeyState(QChar(guessResult.guess[i]), guessResult.letterStates[i]);
    }

    // Handle Game Won or Lost Logic //
    if (guessResult.guessValidity == WordleEngine::GuessValidity::GuessIncorrect) {
        acceptingGameInput = true;
    }
    if (guessResult.guessValidity == WordleEngine::GuessValidity::GameWon || guessResult.guessValidity == WordleEngine::GuessValidity::GameLost) {
        acceptingGameInput = false;
        uiManager->addPopUpMessage(QString::fromStdString(guessResult.message));

        // Determine Game Outcome //
        bool gameWon;
        if (guessResult.guessValidity == WordleEngine::GuessValidity::GameLost) gameWon = false;
        else if (guessResult.guessValidity == WordleEngine::GuessValidity::GameWon) gameWon = true;

        // Track Game Stats, Cache Stats in User Settings, Upload Stats to Statistics Modal //
        settingsManager.updateGameStats(gameWon, guessResult.numAttempts);
        WordleSettings::Statistics newStats = settingsManager.getSettings().statistics;
        uiManager->setStatistics(newStats);
        uiManager->enableRestart();

        // Open Statistics Screen if Game Won //
        if (gameWon) QTimer::singleShot(2000, this, [this, guessResult]() { uiManager->openStatistics(); });
    }
}


/**
 * @brief Handles play again input from the statistics modal.
 */
void WordleApp::handlePlayGameInput() {
    engine.startNewGame();
    uiManager->enableHardModeToggling();
    uiManager->disableRestart();
    uiManager->resetGameUI();
    uiManager->closeIntroScreen();
    uiManager->closeHowToPlay();
    uiManager->closeStatistics();
    uiManager->closeSettings();
    uiManager->resetGameUI();
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
    uiManager->appendLetter(letter);
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
        uiManager->addPopUpMessage(QString::fromStdString(guessResult.message));
        uiManager->invalidGuess();
    }

    // Guess Word Incorrect, Guess Word Correct (Game Won), or Guess Word Incorrect and Out of Turns (Game Lost) //
    else if (guessResult.guessValidity == WordleEngine::GuessValidity::GuessIncorrect ||
             guessResult.guessValidity == WordleEngine::GuessValidity::GameWon ||
             guessResult.guessValidity == WordleEngine::GuessValidity::GameLost
    ) {
        currentGuess = "";
        acceptingGameInput = false;
        uiManager->commitGuess(guessResult.letterStates);
        if (guessResult.numAttempts >= 1 && engine.getDifficulty() == WordleEngine::Difficulty::NORMAL_MODE) uiManager->disableHardModeToggling();
        connect(uiManager, &WordleUI::commitLettersAnimationFinished, this, [this, guessResult]() { initiatePostGuess(guessResult); }, Qt::SingleShotConnection);
    }
}


/**
 * @brief Handles delete input from the keyboard or key events.
 */
void WordleApp::handleDeleteInput() {
    if (!acceptingGameInput) return;
    if (currentGuess.empty()) return;
    currentGuess.pop_back();
    uiManager->deleteLetter();
}
