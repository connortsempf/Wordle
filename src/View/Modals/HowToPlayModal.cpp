/**
 * @file HowToPlayModal.cpp
 * @brief How To Play Modal Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the HowToPlayModal class, which
 * represents the how to play instructions in the Wordle UI.
 */


#include "View/Modals/HowToPlayModal.hpp"


/**
 * @brief Constructs a HowToPlayModal object.
 *
 * @param parent The parent QWidget.
 */
HowToPlayModal::HowToPlayModal(QWidget *parent) : Modal(parent) {
    // Title Text //
    QLabel *title = new QLabel("How to Play", modalContainer);
    title->setContentsMargins(0, 0, 0, 0);
    title->setFont(QFont("NYTKarnak", 24));

    // Sub-Title Text //
    QLabel *subTitle = new QLabel(QString("Guess the Wordle in %1 tries.").arg(GameConfig::NUM_GUESSES), modalContainer);
    subTitle->setContentsMargins(0, 0, 0, 0);
    subTitle->setFont(QFont("NYTStymie", 16));

    // Instructions Text //
    QLabel *instructions = new QLabel(
        "•  Each guess must be a valid five-letter word.\n"
        "•  The color of the tiles will change to show how close your guess was to the word.",
        modalContainer
    );
    instructions->setWordWrap(true);
    instructions->setContentsMargins(0, 0, 0, 0);
    instructions->setFont(QFont("Helvetica Neue", 11, QFont::Normal));
    instructions->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Examples Header Text //
    QLabel *examplesHeader = new QLabel("Examples", modalContainer);
    examplesHeader->setContentsMargins(0, 0, 0, 0);
    examplesHeader->setFont(QFont("Arial", 12, QFont::Bold));

    // Correct Tiles Example Component //
    QHBoxLayout* correctExampleLayout = new QHBoxLayout();
    correctExampleLayout->setSpacing(5);
    correctExampleLayout->setContentsMargins(30, 0, 0, 0);
    correctExampleLayout->setAlignment(Qt::AlignLeft);

    // Correct Tiles Example Text //
    QLabel *correctExampleText = new QLabel("W is in the word and in the correct spot.", modalContainer);
    correctExampleText->setContentsMargins(0, 0, 0, 0);
    correctExampleText->setFont(QFont("Helvetica Neue", 11, QFont::Medium));

    // Present Tiles Example Component //
    QHBoxLayout* presentExampleLayout = new QHBoxLayout();
    presentExampleLayout->setSpacing(5);
    presentExampleLayout->setContentsMargins(30, 0, 0, 0);
    presentExampleLayout->setAlignment(Qt::AlignLeft);

    // Present Tiles Example Text //
    QLabel *presentExampleText = new QLabel("I is in the word but in the wrong spot.", modalContainer);
    presentExampleText->setContentsMargins(0, 0, 0, 0);
    presentExampleText->setFont(QFont("Helvetica Neue", 11, QFont::Medium));
    presentExampleText->setStyleSheet("padding-left: 28px; color: white; border-radius: none; border: none;");

    // Absent Tiles Example Component //
    QHBoxLayout* absentExampleLayout = new QHBoxLayout();
    absentExampleLayout->setSpacing(5);
    absentExampleLayout->setContentsMargins(30, 0, 0, 0);
    absentExampleLayout->setAlignment(Qt::AlignLeft);

    // Absent Tiles Example Text //
    QLabel *absentExampleText = new QLabel("U is not in the word in any spot.", modalContainer);
    absentExampleText->setContentsMargins(0, 0, 0, 0);
    absentExampleText->setFont(QFont("Helvetica Neue", 11, QFont::Medium));
    absentExampleText->setStyleSheet("padding-left: 28px; color: white; border-radius: none; border: none;");

    // Example Tiles Components //
    for (int i = 0; i < 15; i++) {
        QLabel* tile = new QLabel(modalContainer);
        if (i == 0) correctTile = std::move(tile);
        else if (i == 6) presentTile = std::move(tile);
        else if (i == 13) absentTile = std::move(tile);
        tile->setFixedSize(34, 34);
        tile->setAlignment(Qt::AlignCenter);
        tile->setContentsMargins(0, -4, 0, 0);
        tile->setFont(QFont("NYTFranklin", 20, QFont::Bold));
        tile->setText(QString("WORDYLIGHTROGUE"[i]));
        if (i >= 0 && i < 5) correctExampleLayout->addWidget(tile);
        else if (i >= 5 && i < 10) presentExampleLayout->addWidget(tile);
        else if (i >= 10 && i < 15) absentExampleLayout->addWidget(tile);

        connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, tile, i]() {
            const auto& theme = ColorTheme::instance().getTheme();
            QString baseStyle = "border-radius: 0px;";
            QString themeStyle;

            if (i == 0) {
                QColor textColor = theme.guessBoard.tileTextCommitted;
                QColor backgroundColor = theme.indicator.correct;
                themeStyle = QString("border: none; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                    .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                    .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
            }
            else if (i == 6) {
                QColor textColor = theme.guessBoard.tileTextCommitted;
                QColor backgroundColor = theme.indicator.present;
                themeStyle = QString("border: none; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                    .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                    .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
            }
            else if (i == 13) {
                QColor textColor = theme.guessBoard.tileTextCommitted;
                QColor backgroundColor = theme.indicator.absent;
                themeStyle = QString("border: none; color: rgb(%1, %2, %3); background-color: rgb(%4, %5, %6);")
                    .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                    .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
            }
            else {
                QColor textColor = theme.guessBoard.tileTextUncommitted;
                QColor borderColor = theme.guessBoard.tileBorderFilled;
                themeStyle = QString("color: rgb(%1, %2, %3); border: 2px solid rgb(%4, %5, %6);")
                    .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
                    .arg(borderColor.red()).arg(borderColor.green()).arg(borderColor.blue());
            }
            tile->setStyleSheet(baseStyle + themeStyle);
        });
    }

    // Modal Container Layout //
    modalLayout->addWidget(title, 0, Qt::AlignCenter);
    modalLayout->addWidget(subTitle, 0, Qt::AlignCenter);
    modalLayout->addSpacing(15);
    modalLayout->addWidget(instructions);
    modalLayout->addSpacing(15);
    modalLayout->addWidget(examplesHeader);
    modalLayout->addLayout(correctExampleLayout);
    modalLayout->addWidget(correctExampleText);
    modalLayout->addSpacing(10);
    modalLayout->addLayout(presentExampleLayout);
    modalLayout->addWidget(presentExampleText);
    modalLayout->addSpacing(10);
    modalLayout->addLayout(absentExampleLayout);
    modalLayout->addWidget(absentExampleText);
    modalLayout->addSpacing(5);
    modalContainer->adjustSize();

    // Set Theme for Remaining Components //
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, title, subTitle, instructions, examplesHeader, correctExampleText, presentExampleText, absentExampleText]() {
        const auto& theme = ColorTheme::instance().getTheme();
        QColor textColor = theme.text.glyphBackground;
        QString centeredTextStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;")
            .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
        QString indentedTextStyle = QString("color: rgb(%1, %2, %3); padding-left: 28px; border-radius: none; border: none;")
            .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());

        title->setStyleSheet(centeredTextStyle);
        subTitle->setStyleSheet(centeredTextStyle);
        instructions->setStyleSheet(indentedTextStyle);
        examplesHeader->setStyleSheet(indentedTextStyle);
        correctExampleText->setStyleSheet(indentedTextStyle);
        presentExampleText->setStyleSheet(indentedTextStyle);
        absentExampleText->setStyleSheet(indentedTextStyle);
    });

    // Connect Entry Animation Finish to Animation of Example Tiles //
    connect(this, &Modal::entryAnimationFinished, this, &HowToPlayModal::animateExampleTiles);
}


/**
 * @brief Animates the highlighted example tiles on opening of the modal.
 */
void HowToPlayModal::animateExampleTiles() {
    for (int i = 0; i < 3; i++) {
        QLabel* tile;
        if      (i == 0) tile = correctTile;
        else if (i == 1) tile = presentTile;
        else if (i == 2) tile = absentTile;

        tile->setFixedWidth(34);
        tile->setMinimumHeight(0);
        tile->setMaximumHeight(34);

        // Shrink to Zero Height //
        QPropertyAnimation* shrink = new QPropertyAnimation(tile, "maximumHeight");
        shrink->setDuration(225);
        shrink->setStartValue(34);
        shrink->setEndValue(0);
        shrink->setEasingCurve(QEasingCurve::InQuad);

        // Grow Back to Full Height //
        QPropertyAnimation* grow = new QPropertyAnimation(tile, "maximumHeight");
        grow->setDuration(225);
        grow->setStartValue(0);
        grow->setEndValue(34);
        grow->setEasingCurve(QEasingCurve::OutQuad);

        // Run Shrink and Grow Sequentially //
        QSequentialAnimationGroup* sequence = new QSequentialAnimationGroup(tile);
        sequence->addAnimation(shrink);
        sequence->addAnimation(grow);

        connect(sequence, &QSequentialAnimationGroup::finished, tile, [tile]() { tile->setFixedSize(34, 34); });
        sequence->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
