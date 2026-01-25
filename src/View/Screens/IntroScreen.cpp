/**
 * @file IntroScreen.cpp
 * @brief IntroScreen Screen Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the IntroScreen class, which represents
 * the entire screen and scene for the IntroScreen Screen within the Wordle App.
 */


#include "View/Screens/IntroScreen.hpp"


/**
 * @brief Constructs a IntroScreen object.
 *
 * @param parent The parent QWidget.
 */
IntroScreen::IntroScreen(QWidget *parent) : QWidget(parent) {
    // Construct Header Image //
    QLabel* headerImageLabel = new QLabel(this);
    QPixmap headerImage("../assets/textures/wordle-emblem-transparent.png");
    headerImageLabel->setPixmap(headerImage);
    headerImageLabel->setFixedSize(90, 90);
    headerImageLabel->setScaledContents(true);
    headerImageLabel->setAlignment(Qt::AlignCenter);
    headerImageLabel->setContentsMargins(0, 0, 0, 0);

    // Construct Title //
    QLabel* title = new QLabel("Wordle", this);
    title->setAlignment(Qt::AlignCenter);
    title->setContentsMargins(0, 0, 0, 0);
    title->setFont(QFont("NYTKarnak", 42));
    title->setStyleSheet("color: black;");

    // Construct Directions //
    QLabel* directions = new QLabel(QString("Get %1 changes to guess\na %2-letter word.").arg(GameConfig::NUM_GUESSES).arg(GameConfig::WORD_LENGTH), this);
    directions->setAlignment(Qt::AlignCenter);
    directions->setContentsMargins(0, 0, 0, 0);
    directions->setFont(QFont("NYTStymie", 25, QFont::Medium));
    directions->setStyleSheet("color: black;");

    // Construct Play Button //
    QPushButton* playButton = new QPushButton("Play", this);
    playButton->setCursor(Qt::PointingHandCursor);
    playButton->setFixedSize(180, 44);
    playButton->setContentsMargins(0, 0, 0, 0);
    playButton->setFont(QFont("Helvetica Neue", 12, QFont::Medium));
    playButton->setStyleSheet(
        "QPushButton { border-radius: 22px; border: 2px solid rgb(0, 0, 0); background-color: rgba(0, 0, 0, 0); color: black; }"
        "QPushButton::hover { border: none; background-color: rgb(0, 0, 0); color: white; }"
        "QPushButton::pressed { border: none; background-color: rgb(0, 0, 0); color: white; }"
    );
    connect(playButton, &QPushButton::clicked, this, &IntroScreen::playInput);

    // Credits //
    QLabel* credits = new QLabel("2026\nDeveloped by Connor Sempf", this);
    credits->setAlignment(Qt::AlignCenter);
    credits->setContentsMargins(0, 0, 0, 0);
    credits->setFont(QFont("Helvetica Neue", 10, QFont::Medium));
    credits->setStyleSheet("color: black;");

    // Construct Layout //
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch();
    layout->addWidget(headerImageLabel, 0, Qt::AlignCenter);
    layout->addSpacing(5);
    layout->addWidget(title, 0, Qt::AlignCenter);
    layout->addWidget(directions, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(playButton, 0, Qt::AlignCenter);
    layout->addSpacing(30);
    layout->addWidget(credits, 0, Qt::AlignCenter);
    layout->addStretch();

    setLayout(layout);
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: rgb(227, 227, 225);");
};
