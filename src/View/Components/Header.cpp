/**
 * @file Header.cpp
 * @brief Header Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the Header class, which
 * represents the Wordle UI header.
 */


#include "View/Components/Header.hpp"


/**
 * @brief The width of each header button.
 */
const unsigned int Header::BUTTON_WIDTH = 56;

/**
 * @brief The height of each header button.
 */
const unsigned int Header::BUTTON_HEIGHT = 56;


/**
 * @brief Constructs a Header object.
 *
 * @param parent The parent QWidget.
 */
Header::Header(QWidget *parent) : QWidget(parent) {
    QString buttonStyleSheet =
        "QPushButton { border: none; border-radius: 0px;}"
        "QPushButton:hover { background-color: rgb(54, 54, 54); border: none; border-radius: 0px; }"
        "QPushButton:pressed { background-color: rgb(54, 54, 54); border: none; border-radius: 0px; }";

    // Title //
    QLabel* title = new QLabel("Wordle", this);
    title->setAlignment(Qt::AlignCenter);
    title->setContentsMargins(0, 0, 0, 0);
    title->setFont(QFont("NYTKarnak", 30));

    // Statistics Button //
    QPushButton* statisticsButton = new QPushButton(this);
    statisticsButton->setIcon(QIcon(":/images/assets/images/statistics-icon.png"));
    statisticsButton->setIconSize(QSize(28, 28));
    statisticsButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    statisticsButton->setStyleSheet(buttonStyleSheet);
    statisticsButton->setCursor(Qt::PointingHandCursor);
    connect(statisticsButton, &QPushButton::clicked, this, [this]() { emit openStatistics(); });

    // How To Play Button //
    QPushButton* howToPlayButton = new QPushButton(this);
    howToPlayButton->setIcon(QIcon(":/images/assets/images/how-to-play-icon.png"));
    howToPlayButton->setIconSize(QSize(28, 28));
    howToPlayButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    howToPlayButton->setStyleSheet(buttonStyleSheet);
    howToPlayButton->setCursor(Qt::PointingHandCursor);
    connect(howToPlayButton, &QPushButton::clicked, this, [this]() { emit openHowToPlay(); });

    // Settings Button //
    QPushButton* settingsButton = new QPushButton(this);
    settingsButton->setIcon(QIcon(":/images/assets/images/settings-icon.png"));
    settingsButton->setIconSize(QSize(28, 28));
    settingsButton->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    settingsButton->setStyleSheet(buttonStyleSheet);
    settingsButton->setCursor(Qt::PointingHandCursor);
    connect(settingsButton, &QPushButton::clicked, this, [this]() { emit openSettings(); });

    // Divider //
    QFrame* divider = new QFrame(this);
    divider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);

    // Buttons Layout //
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(statisticsButton);
    buttonLayout->addWidget(howToPlayButton);
    buttonLayout->addWidget(settingsButton);
    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setLayout(buttonLayout);

    // Spacer for Left Side (Balance the Buttons on Right) //
    QWidget* leftSpacer = new QWidget(this);
    leftSpacer->setFixedWidth(buttonContainer->sizeHint().width());

    // Content Layout //
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(leftSpacer);
    contentLayout->addStretch();
    contentLayout->addWidget(title);
    contentLayout->addStretch();
    contentLayout->addWidget(buttonContainer);

    // Main Layout //
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 10);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(divider);

    // Set Theme //
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, title, statisticsButton, howToPlayButton, settingsButton, divider]() {
        const auto& theme = ColorTheme::instance().getTheme();

        // Color Title Label //
        QColor titleColor = theme.text.glyphBackground;
        QString titleStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(titleColor.red()).arg(titleColor.green()).arg(titleColor.blue());
        title->setStyleSheet(titleStyle);

        // Header Button Dynamic Color //
        QColor buttonColor = theme.headerButton.dynamicBackground;
        QString buttonStyle = QString(
            "QPushButton { border: none; border-radius: 0px;}"
            "QPushButton:hover { background-color: rgb(%1, %2, %3); border: none; border-radius: 0px; }"
            "QPushButton:pressed { background-color: rgb(%4, %5, %6); border: none; border-radius: 0px; }"
        ).arg(buttonColor.red()).arg(buttonColor.green()).arg(buttonColor.blue()).arg(buttonColor.red()).arg(buttonColor.green()).arg(buttonColor.blue());
        statisticsButton->setStyleSheet(buttonStyle);
        howToPlayButton->setStyleSheet(buttonStyle);
        settingsButton->setStyleSheet(buttonStyle);

        // Recolor the Header Buttons //
        QColor iconColor = theme.texture.glyphBackground;
        for (int i = 0; i < 3; i++) {
            QPixmap closeButtonOriginal;
            if      (i == 0) closeButtonOriginal = QPixmap(":/images/assets/images/statistics-icon.png");
            else if (i == 1) closeButtonOriginal = QPixmap(":/images/assets/images/how-to-play-icon.png");
            else if (i == 2) closeButtonOriginal = QPixmap(":/images/assets/images/settings-icon.png");
            QPixmap closeButtonPainted = closeButtonOriginal;
            closeButtonPainted.fill(Qt::transparent);
            QPainter painter(&closeButtonPainted);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawPixmap(0, 0, closeButtonOriginal);
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(closeButtonPainted.rect(), iconColor);
            painter.end();
            if      (i == 0) statisticsButton->setIcon(QIcon(closeButtonPainted));
            else if (i == 1) howToPlayButton->setIcon(QIcon(closeButtonPainted));
            else if (i == 2) settingsButton->setIcon(QIcon(closeButtonPainted));
        }

        // Divider Color //
        QColor dividerColor = theme.divider.background;
        QString dividerStyle = QString("background-color: rgb(%1, %2, %3); border: none; border-radius: none;").arg(dividerColor.red()).arg(dividerColor.green()).arg(dividerColor.blue());
        divider->setStyleSheet(dividerStyle);
    });

    // Set Layout //
    setLayout(mainLayout);
}
