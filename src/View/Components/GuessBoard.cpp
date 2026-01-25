/**
 * @file GuessBoard.cpp
 * @brief Guess Board Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the GuessBoard class, which
 * represents a single row of tiles in the Wordle UI.
 */


#include "View/Components/GuessBoard.hpp"


/**
 * @brief Constructs a GuessBoard object.
 *
 * @param parent The parent QWidget.
 */
GuessBoard::GuessBoard(QWidget *parent) : QWidget(parent) {
    // Construct Layout //
    QGridLayout* layout = new QGridLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(5);

    // Construct Letter Tiles //
    for (int i = 0; i < letterTiles.size(); i++) {
        for (int j = 0; j < letterTiles[0].size(); j++) {
            std::tuple<QLabel*, TileState, LetterState>& tile = letterTiles[i][j];
            QLabel* letterTile = new QLabel(this);
            tile = {letterTile, TileState::EMPTY, LetterState::ABSENT};
            letterTile->setFixedSize(62, 62);
            letterTile->setAlignment(Qt::AlignCenter);
            letterTile->setContentsMargins(0, -5, 0, 0);
            letterTile->setFont(QFont("NYTFranklin", 30, QFont::Bold));
            layout->addWidget(letterTile, i, j);
            updateLetterTileStyle(letterTiles[i][j]);
            connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, i, j]() { updateLetterTileStyle(letterTiles[i][j]); });
        }
    }

    setContentsMargins(10, 10, 10, 10);
    setLayout(layout);
}


/**
 * @brief Reset the logical and visual state for a new game.
 */
void GuessBoard::reset() {
    activeRow = 0;
    activeColumn = 0;

    for (auto& letterTileRow : letterTiles) {
        for (auto& letterTile : letterTileRow) {
            std::get<0>(letterTile)->setText("");
            std::get<1>(letterTile) = TileState::EMPTY;
            std::get<2>(letterTile) = LetterState::ABSENT;
            updateLetterTileStyle(letterTile);
        }
    }
}


/**
 * @brief Appends a letter to the current row.
 *
 * @param letter The letter to append.
 */
void GuessBoard::appendLetter(QChar letter) {
    if (activeColumn >= GameConfig::WORD_LENGTH) return;
    std::tuple<QLabel*, TileState, LetterState>& letterTile = letterTiles[activeRow][activeColumn];
    std::get<0>(letterTile)->setText(letter);
    std::get<1>(letterTile) = TileState::FILLED;
    animateAppendLetter(letterTile, activeColumn++);
}


/**
 * @brief Deletes a letter from the current row.
 */
void GuessBoard::deleteLetter() {
    if (activeColumn <= 0) return;
    --activeColumn;
    std::get<0>(letterTiles[activeRow][activeColumn])->setText("");
    std::get<1>(letterTiles[activeRow][activeColumn]) = TileState::EMPTY;
    updateLetterTileStyle(letterTiles[activeRow][activeColumn]);
}


/**
 * @brief Commits the letters of a guess in the row with their respective validities.
 *
 * @param letterStates An array of letter states for each tile.
 *
 * @return True if the commit was successful, false otherwise.
 */
void GuessBoard::commitGuess(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates) {
    if (activeRow >= GameConfig::NUM_GUESSES) return;
    for (int i = 0; i < GameConfig::WORD_LENGTH; i++) {
        auto& letterTile = letterTiles[activeRow][i];
        std::get<1>(letterTile) = TileState::COMMITTED;
        std::get<2>(letterTile) = letterStates[i];
    }
    animateCommitLetters(letterStates, activeRow++);
    activeColumn = 0;
}


/**
 * @brief Response to an invalid guess occurrance.
 */
void GuessBoard::invalidGuess() {
    if (shakeAnimation && shakeAnimation->state() == QAbstractAnimation::Running) return;
    animateInvalidGuess(activeRow);
}


/**
 * @brief Updates the visual style of the tile based on its state.
 *
 * @param letterTile The letter tile to update.
 */
void GuessBoard::updateLetterTileStyle(std::tuple<QLabel*, TileState, LetterState> letterTile) {
    const ColorTheme::Theme& theme = ColorTheme::instance().getTheme();
    QColor textColor;
    QColor styleColor;
    QString letterTileStyle;

    if (std::get<1>(letterTile) == TileState::EMPTY) {
        textColor = theme.guessBoard.tileTextUncommitted;
        styleColor = theme.guessBoard.tileBorderEmpty;
        letterTileStyle = QString("color: rgb(%1, %2, %3); border: 2px solid rgb(%4, %5, %6);")
            .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
            .arg(styleColor.red()).arg(styleColor.green()).arg(styleColor.blue());
    }
    else if (std::get<1>(letterTile) == TileState::FILLED) {
        textColor = theme.guessBoard.tileTextUncommitted;
        styleColor = theme.guessBoard.tileBorderFilled;
        letterTileStyle = QString("color: rgb(%1, %2, %3); border: 2px solid rgb(%4, %5, %6);")
            .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
            .arg(styleColor.red()).arg(styleColor.green()).arg(styleColor.blue());
    }
    else if (std::get<1>(letterTile) == TileState::COMMITTED) {
        if (std::get<2>(letterTile) == LetterState::ABSENT) {
            textColor = theme.guessBoard.tileTextCommitted;
            styleColor = theme.indicator.absent;
            letterTileStyle = QString("color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                .arg(styleColor.red()).arg(styleColor.green()).arg(styleColor.blue());
        }
        else if (std::get<2>(letterTile) == LetterState::PRESENT) {
            textColor = theme.guessBoard.tileTextCommitted;
            styleColor = theme.indicator.present;
            letterTileStyle = QString("color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                .arg(styleColor.red()).arg(styleColor.green()).arg(styleColor.blue());
        }
        else if (std::get<2>(letterTile) == LetterState::CORRECT) {
            textColor = theme.guessBoard.tileTextCommitted;
            styleColor = theme.indicator.correct;
            letterTileStyle = QString("color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                .arg(styleColor.red()).arg(styleColor.green()).arg(styleColor.blue());
        }
    }

    std::get<0>(letterTile)->setStyleSheet(letterTileStyle);
}


/**
 * @brief Animate the letter tile when a letter is appended.
 *
 * @param letterTile The letter tile to animate.
 * @param activeColumn The active column in the active row.
 */
void GuessBoard::animateAppendLetter(std::tuple<QLabel*, TileState, LetterState> letterTile, unsigned int activeColumn) {
    QLabel* tile = std::get<0>(letterTile);

    // Expand Animation Geometry //
    tile->setMinimumSize(62, 62);
    tile->setMaximumSize(tile->width() + (2 * animateAppendLetterExpansion), tile->height() + (2 * animateAppendLetterExpansion));
    QRect startGeometry = tile->geometry();
    QRect endGeometry = startGeometry.adjusted(-animateAppendLetterExpansion, -animateAppendLetterExpansion, animateAppendLetterExpansion, animateAppendLetterExpansion);
    endGeometry.moveCenter(startGeometry.center());

    // Expand Animation //
    QPropertyAnimation* expandAnimation = new QPropertyAnimation(tile, "geometry");
    expandAnimation->setDuration(80);
    expandAnimation->setStartValue(startGeometry);
    expandAnimation->setKeyValueAt(0.5, endGeometry);
    expandAnimation->setEndValue(startGeometry);
    expandAnimation->setEasingCurve(QEasingCurve::OutInQuad);
    connect(expandAnimation, &QPropertyAnimation::finished, this, [this, letterTile]() {
        std::get<0>(letterTile)->setFixedSize(62, 62);
        updateLetterTileStyle(letterTile);
    });
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}


/**
 * @brief Animate the active row tiles when an invalid guess occurs.
 *
 * @param activeRow The active row to animate.
 */
void GuessBoard::animateInvalidGuess(unsigned int activeRow) {
    if (shakeAnimation) delete shakeAnimation;
    shakeAnimation = new QParallelAnimationGroup(this);

    for (int i = 0; i < GameConfig::WORD_LENGTH; i++) {
        QLabel* letterTile = std::get<0>(letterTiles[activeRow][i]);

        // Get the current position
        QPoint startPos = letterTile->pos();

        // Create shake animation for this tile
        QPropertyAnimation* shake = new QPropertyAnimation(letterTile, "pos");
        shake->setDuration(400);
        shake->setStartValue(startPos);
        shake->setKeyValueAt(0.1, startPos + QPoint(8, 0));   // Right
        shake->setKeyValueAt(0.3, startPos + QPoint(-8, 0));  // Left
        shake->setKeyValueAt(0.5, startPos + QPoint(8, 0));   // Right
        shake->setKeyValueAt(0.7, startPos + QPoint(-8, 0));  // Left
        shake->setKeyValueAt(0.9, startPos + QPoint(4, 0));   // Slight right
        shake->setEndValue(startPos);                         // Back to start
        shake->setEasingCurve(QEasingCurve::InOutSine);
        shakeAnimation->addAnimation(shake);
    }
    shakeAnimation->start();
}


/**
 * @brief Animate the letter tiles of the active row when letters are committed.
 *
 * @param letterStates The states of committed letters.
 * @param activeRow The active row of the letter commitment.
 */
void GuessBoard::animateCommitLetters(const std::array<LetterState, GameConfig::WORD_LENGTH>& letterStates, unsigned int activeRow) {
    QParallelAnimationGroup* letterTilesAnimation = new QParallelAnimationGroup(this);

    for (int i = 0; i < GameConfig::WORD_LENGTH; i++) {
        // Letter Tile //
        std::tuple<QLabel*, TileState, LetterState>& currentLetterTile = letterTiles[activeRow][i];
        QLabel* currentTile = std::get<0>(currentLetterTile);
        // LetterState currentLetterState = letterStates[i];
        // std::get<2>(currentLetterTile) = currentLetterState;
        currentTile->setFixedWidth(62);
        currentTile->setMinimumHeight(0);
        currentTile->setMaximumHeight(62);

        // Shrink Animation //
        QPropertyAnimation* shrinkAnimation = new QPropertyAnimation(currentTile, "maximumHeight");
        shrinkAnimation->setDuration(300);
        shrinkAnimation->setStartValue(62);
        shrinkAnimation->setEndValue(0);
        shrinkAnimation->setEasingCurve(QEasingCurve::InQuad);
        connect(shrinkAnimation, &QPropertyAnimation::finished, this, [this, currentLetterTile]() {
            updateLetterTileStyle(currentLetterTile);
        });

        // Grow Animation //
        QPropertyAnimation* growAnimation = new QPropertyAnimation(currentTile, "maximumHeight");
        growAnimation->setDuration(300);
        growAnimation->setStartValue(0);
        growAnimation->setEndValue(62);
        growAnimation->setEasingCurve(QEasingCurve::OutQuad);
        connect(growAnimation, &QPropertyAnimation::finished, this, [this, currentTile]() {
            currentTile->setFixedSize(62, 62);
        });

        // Animation Sequence for Each Letter Tile (Delay, Shrink Grow) //
        QSequentialAnimationGroup* delayedLetterTileAnimation = new QSequentialAnimationGroup();
        delayedLetterTileAnimation->addPause(i * 300);
        delayedLetterTileAnimation->addAnimation(shrinkAnimation);
        delayedLetterTileAnimation->addAnimation(growAnimation);
        letterTilesAnimation->addAnimation(delayedLetterTileAnimation);
    }

    connect(letterTilesAnimation, &QSequentialAnimationGroup::finished, this, [this, letterStates, activeRow]() {
        bool correctGuess = true;
        for (const auto& letterState : letterStates) {
            if (letterState != LetterState::CORRECT) correctGuess = false;
        }
        if (correctGuess) animateCorrectGuess(activeRow);
        emit commitLettersAnimationFinished();
    });
    letterTilesAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}


/**
 * @brief Animate the active row tiles when the guess is correct.
 *
 * @param activeRow The active row to animate.
 */
void GuessBoard::animateCorrectGuess(unsigned int activeRow) {
    QParallelAnimationGroup* sinusoidalBounceAnimation = new QParallelAnimationGroup(this);

    for (int i = 0; i < GameConfig::WORD_LENGTH; i++) {
        QLabel* letterTile = std::get<0>(letterTiles[activeRow][i]);
        QPoint startPos = letterTile->pos();

        // Add Staggered Delay Based on Left-To-Right Position //
        QSequentialAnimationGroup* delayedBounce = new QSequentialAnimationGroup();
        QPauseAnimation* delay = new QPauseAnimation(i * 100);
        delayedBounce->addPause(i * 100);

        // Create Spring Bounce Animation //
        QPropertyAnimation* bounce = new QPropertyAnimation(letterTile, "pos");
        bounce->setDuration(800);
        bounce->setStartValue(startPos);

        // Spring Motion with Overshoot //
        bounce->setKeyValueAt(0.22, startPos + QPoint(0, -25)); // Up (less high)
        bounce->setKeyValueAt(0.44, startPos + QPoint(0, 10));  // Below rest (gentler overshoot)
        bounce->setKeyValueAt(0.66, startPos + QPoint(0, -3));  // Up again (even smaller)
        bounce->setKeyValueAt(0.88, startPos + QPoint(0, 1));   // Tiny overshoot
        bounce->setEndValue(startPos);                          // Settle at rest
        bounce->setEasingCurve(QEasingCurve::OutCubic);         // Smoother easing
        delayedBounce->addAnimation(bounce);

        // Add to Staggered Parallel Animation Group //
        sinusoidalBounceAnimation->addAnimation(delayedBounce);
    }

    // connect(sinusoidalBounceAnimation, &QParallelAnimationGroup::finished, this, &GuessBoard::victoryAnimationFinished);
    sinusoidalBounceAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
