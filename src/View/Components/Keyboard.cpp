/**
 * @file Keyboard.cpp
 * @brief Keyboard Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the Keyboard class, which
 * represents the Wordle UI keyboard.
 */


#include "View/Components/Keyboard.hpp"


/**
 * @brief The width of each key button.
 */
const unsigned int Keyboard::KEY_WIDTH = 45;

/**
 * @brief The height of each key button.
 */
const unsigned int Keyboard::KEY_HEIGHT = 60;

/**
 * @brief The spacing between key buttons.
 */
const unsigned int Keyboard::KEY_SPACING = 6;

/**
 * @brief The width of the function key buttons.
 */
const unsigned int Keyboard::FUNCTION_KEY_WIDTH = (KEY_WIDTH * 1.5) + (KEY_SPACING * 0.5);

/**
 * @brief The layout of the keyboard button keys.
 */
const std::vector<std::vector<QString>> Keyboard::KEY_BUTTON_TEXTS = {
    {     "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P" },
    {     "A", "S", "D", "F", "G", "H", "J", "K", "L",     },
    { "ENTER", "Z", "X", "C", "V", "B", "N", "M", "\uE94F" }
};


/**
 * @brief Constructs a Keyboard object.
 *
 * @param parent The parent QWidget.
 */
Keyboard::Keyboard(QWidget *parent) : QWidget(parent) {
    keys.reserve(KEY_BUTTON_TEXTS.size());

    // Layout Key Button Rows //
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(KEY_SPACING);
    layout->setContentsMargins(15, 20, 15, 15);
    layout->setAlignment(Qt::AlignCenter);

    // Create Key Buttons //
    for (int i = 0; i < KEY_BUTTON_TEXTS.size(); i++) {
        keys.emplace_back();

        // Key Button Row Layout //
        QHBoxLayout* keyRowLayout = new QHBoxLayout();
        keyRowLayout->setSpacing(5);
        keyRowLayout->setContentsMargins(0, 0, 0, 0);
        keyRowLayout->setAlignment(Qt::AlignCenter);

        for (int j = 0; j < KEY_BUTTON_TEXTS[i].size(); j++) {
            const QString& keyButtonText = KEY_BUTTON_TEXTS[i][j];
            auto key = std::tuple<QPushButton*, Keyboard::CommitState, LetterState>(new QPushButton(QString(keyButtonText), this), Keyboard::CommitState::UNCOMMITTED, LetterState::ABSENT);
            keys[i].push_back(key);
            QPushButton* keyButton = std::get<0>(keys[i][j]);

            // Set Geometry and Connect Signals //
            if (keyButtonText == "ENTER") {
                keyButton->setFixedSize(FUNCTION_KEY_WIDTH, KEY_HEIGHT);
                connect(keyButton, &QPushButton::clicked, this, &Keyboard::enterInput);
            }
            else if (keyButtonText == "\uE94F") {
                keyButton->setFixedSize(FUNCTION_KEY_WIDTH, KEY_HEIGHT);
                connect(keyButton, &QPushButton::clicked, this, &Keyboard::backspaceInput);
            }
            else {
                keyButton->setFixedSize(KEY_WIDTH, KEY_HEIGHT);
                connect(keyButton, &QPushButton::clicked, this, [this, keyButtonText]() { emit letterInput(keyButtonText[0]); });
            }

            keyButton->setCursor(Qt::PointingHandCursor);
            keyRowLayout->addWidget(keyButton);
            connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, i, j]() { updateStyle(keys[i][j]); });
            updateStyle(keys[i][j]);
        }
        layout->addLayout(keyRowLayout);
    }

    // Set the Layout //
    setLayout(layout);
}


/**
 * @brief Reset the visual state for a new game.
 */
void Keyboard::reset() {
    for (auto& keyRow : keys) {
        for (auto& key : keyRow) {
            std::get<1>(key) = Keyboard::CommitState::UNCOMMITTED;
            std::get<2>(key) = LetterState::ABSENT;
            updateStyle(key);
        }
    }
}


/**
 * @brief Set the logical state for a key button.
 *
 * @param keyLetter The key letter.
 * @param letterState The visual state to set (ABSENT, PRESENT, CORRECT).
 */
void Keyboard::setKeyState(QChar keyLetter, LetterState letterState) {
    for (auto& keyRow : keys) {
        for (auto& key : keyRow) {
            if (std::get<0>(key)->text()[0] == keyLetter.toUpper()) {
                std::get<1>(key) = Keyboard::CommitState::COMMITTED;
                std::get<2>(key) = letterState;
                updateStyle(key);
                return;
            }
        }
    }
}


/**
 * @brief Update the visual style for a given key button.
 *
 * @param key The key button to update.
 */
void Keyboard::updateStyle(const std::tuple<QPushButton*, Keyboard::CommitState, LetterState>& key) {
    QPushButton* keyButton = std::get<0>(key);
    bool keyButtonCommitted = std::get<1>(key) == Keyboard::CommitState::COMMITTED;
    LetterState keyButtonLetterState = std::get<2>(key);
    const ColorTheme::Theme& theme = ColorTheme::instance().getTheme();
    QColor textColor;
    QColor backgroundColor;
    QString keyStyle;
    QFont keyFont;

    if (keyButton->text() == "ENTER") {
        keyFont = QFont("NYTFranklin", 12, QFont::Bold);
        textColor = theme.keyboard.keyTextUncommitted;
        backgroundColor = theme.keyboard.keyBackground;
    }
    else if (keyButton->text() == "\uE94F") {
        keyFont = QFont("NYTFranklin", 18, QFont::Bold);
        textColor = theme.keyboard.keyTextUncommitted;
        backgroundColor = theme.keyboard.keyBackground;
    }
    else {
        keyFont = QFont("NYTFranklin", 18, QFont::Bold);
        if (!keyButtonCommitted) {
            textColor = theme.keyboard.keyTextUncommitted;
            backgroundColor = theme.keyboard.keyBackground;
        }
        else if (keyButtonLetterState == LetterState::ABSENT) {
            textColor = theme.keyboard.keyTextCommitted;
            backgroundColor = theme.indicator.absent;
        }
        else if (keyButtonLetterState == LetterState::PRESENT) {
            textColor = theme.keyboard.keyTextCommitted;
            backgroundColor = theme.indicator.present;
        }
        else if (keyButtonLetterState == LetterState::CORRECT) {
            textColor = theme.keyboard.keyTextCommitted;
            backgroundColor = theme.indicator.correct;
        }
    }

    keyStyle = QString("border-radius: 4px; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
        .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());

    keyButton->setStyleSheet(keyStyle);
    keyButton->setFont(keyFont);
}
