/**
 * @file StatisticsModal.cpp
 * @brief Statistics Modal Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the StatisticsModal class, which
 * represents the statistics in the Wordle UI.
 */


#include "View/Modals/StatisticsModal.hpp"


/**
 * @brief Constructs a StatisticsModal object.
 *
 * @param parent The parent QWidget.
 */
StatisticsModal::StatisticsModal(QWidget *parent) : Modal(parent) {
    // Title Text //
    QLabel *title = new QLabel("Statistics", modalContainer);
    title->setAlignment(Qt::AlignCenter);
    title->setContentsMargins(0, 0, 0, 0);
    title->setFont(QFont("NYTKarnak", 24));

    // Statistics Texts Layout //
    QVBoxLayout* statsLayout = new QVBoxLayout();
    statsLayout->setAlignment(Qt::AlignCenter);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(8);

    // Games Played Statistics Texts //
    QHBoxLayout* statTextLayout = new QHBoxLayout();
    statTextLayout->setAlignment(Qt::AlignCenter);
    statTextLayout->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 4; i++) {
        headerStatLabels[i] = new QLabel("0", modalContainer);
        QLabel* headerStatLabel = headerStatLabels[i];
        headerStatLabels[i]->setFixedWidth(60);
        headerStatLabels[i]->setAlignment(Qt::AlignCenter);
        headerStatLabels[i]->setContentsMargins(0, 0, 0, 0);
        headerStatLabels[i]->setFont(QFont("Arial", 28, QFont::Normal));
        statTextLayout->addWidget(headerStatLabels[i]);

        // Set Theme //
        connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, headerStatLabel]() {
            const auto& theme = ColorTheme::instance().getTheme();
            QColor textColor = theme.text.glyphBackground;
            QString headerStatTextStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
            headerStatLabel->setStyleSheet(headerStatTextStyle);
        });
    }
    statsLayout->addLayout(statTextLayout);

    std::vector<QString> statsSubTextLabels = { "Played", "Win %", "Current Streak", "Max Streak" };
    QHBoxLayout* statSubtextsLayout = new QHBoxLayout();
    statSubtextsLayout->setAlignment(Qt::AlignCenter);
    statSubtextsLayout->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 4; i++) {
        QLabel *statSubText = new QLabel(statsSubTextLabels[i], modalContainer);
        statSubText->setFixedWidth(60);
        statSubText->setWordWrap(true);
        statSubText->setContentsMargins(0, 0, 0, 0);
        statSubText->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        statSubText->setFont(QFont("Arial", 10, QFont::Medium));
        statSubtextsLayout->addWidget(statSubText);

        // Set Theme //
        connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, statSubText]() {
            const auto& theme = ColorTheme::instance().getTheme();
            QColor textColor = theme.text.glyphBackground;
            QString statSubTextStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
            statSubText->setStyleSheet(statSubTextStyle);
        });
    }
    statsLayout->addLayout(statSubtextsLayout);

    // Distribution Text //
    QLabel *distributionText = new QLabel("GUESS DISTRIBUTION", modalContainer);
    distributionText->setAlignment(Qt::AlignCenter);
    distributionText->setContentsMargins(0, 5, 0, 5);
    distributionText->setFont(QFont("Arial", 14, QFont::Bold));

    // Guess Distributions Stats Layout //
    guessDistributionsStatsLayout = new QVBoxLayout();
    guessDistributionsStatsLayout->setAlignment(Qt::AlignTop | Qt::AlignVCenter);
    guessDistributionsStatsLayout->setContentsMargins(0, 0, 0, 20);
    guessDistributionsStatsLayout->setSpacing(4);

    // Guess Distributions Stats Components //
    for (int i = 0; i < GameConfig::NUM_GUESSES; i++) {
        QLabel *guessAmountText = new QLabel(QString::number(i + 1), modalContainer);
        guessAmountText->setFixedSize(45, 24);
        guessAmountText->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        guessAmountText->setContentsMargins(0, 0, 0, 0);
        guessAmountText->setFont(QFont("Arial", 11, QFont::Normal));

        guessDistributionBars[i] = new QLabel("0", modalContainer);
        QLabel* guessDistributionBar = guessDistributionBars[i];
        guessDistributionBars[i]->setFixedWidth(25);
        guessDistributionBars[i]->setFixedHeight(24);
        guessDistributionBars[i]->setContentsMargins(8, 0, 8, 0);
        guessDistributionBars[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        guessDistributionBars[i]->setFont(QFont("Arial", 11, QFont::Bold));
        guessDistributionBars[i]->setStyleSheet("color: white; background-color: rgb(58, 58, 60); border-radius: none; border: none;");

        // Set Theme //
        connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, guessAmountText, guessDistributionBar]() {
            const auto& theme = ColorTheme::instance().getTheme();
            QColor textColor = theme.text.glyphBackground;
            QString guessAmountTextStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
            guessAmountText->setStyleSheet(guessAmountTextStyle);

            QColor barColor = theme.distributionBars.background;
            QString barStyle = QString("background-color: rgb(%1, %2, %3); color: white; border-radius: none; border: none;").arg(barColor.red()).arg(barColor.green()).arg(barColor.blue());
            guessDistributionBar->setStyleSheet(barStyle);
        });

        QHBoxLayout* statLayout = new QHBoxLayout();
        statLayout->setAlignment(Qt::AlignTop);
        statLayout->setContentsMargins(0, 2, 0, 2);
        statLayout->addWidget(guessAmountText);
        statLayout->addWidget(guessDistributionBars[i]);
        guessDistributionsStatsLayout->addLayout(statLayout);
    }

    // Wrap in a Fixed-Height Container Widget //
    QWidget* guessDistributionsWidget = new QWidget(modalContainer);
    guessDistributionsWidget->setFixedHeight(174);
    guessDistributionsWidget->setLayout(guessDistributionsStatsLayout);
    guessDistributionsWidget->setStyleSheet("background-color: rgba(0, 0, 0, 0); border: none;");

    // PLay Again Button //
    QPushButton* playAgainButton = new QPushButton("PLAY AGAIN!", modalContainer);
    playAgainButton->setFixedSize(250, 55);
    playAgainButton->setLayoutDirection(Qt::RightToLeft);
    playAgainButton->setCursor(Qt::PointingHandCursor);
    playAgainButton->setIcon(QIcon("../assets/textures/refresh-icon.png"));
    playAgainButton->setIconSize(QSize(20, 20));
    playAgainButton->setContentsMargins(0, 0, 0, 0);
    playAgainButton->setFont(QFont("Arial", 18, QFont::Bold));
    connect(playAgainButton, &QPushButton::clicked, this, [this]() { emit playAgain(); });
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, playAgainButton]() {
        const auto& theme = ColorTheme::instance().getTheme();
        QColor backgroundColor = theme.indicator.correct;
        QString playAgainButtonStyle = QString(
            "text-align: left; padding-left: 30px; padding-right: 30px; spacing: 5px; border-radius: 8px;"
            "border: none; color: white; background-color: rgb(%1, %2, %3);"
        ).arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
        playAgainButton->setStyleSheet(playAgainButtonStyle);
    });

    // PLay Again Button Layout //
    playAgainButtonLayout = new QHBoxLayout();
    playAgainButtonLayout->setContentsMargins(0, 10, 0, 15);
    playAgainButtonLayout->setAlignment(Qt::AlignCenter);
    playAgainButtonLayout->addWidget(playAgainButton);

    // Add to Modal Container Layout //
    modalLayout->addWidget(title);
    modalLayout->addSpacing(8);
    modalLayout->addLayout(statsLayout);
    modalLayout->addSpacing(15);
    modalLayout->addWidget(distributionText);
    modalLayout->addSpacing(5);
    modalLayout->addWidget(guessDistributionsWidget);
    modalLayout->addLayout(playAgainButtonLayout);
    modalContainer->adjustSize();

    // Set Theme for Remaining Components //
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, title, distributionText]() {
        const auto& theme = ColorTheme::instance().getTheme();
        QColor textColor = theme.text.glyphBackground;
        QString titleStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
        title->setStyleSheet(titleStyle);
        QString guessDistributionTextStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
        distributionText->setStyleSheet(guessDistributionTextStyle);
    });
}


/**
 * @brief Set the statistics metrics for the UI to display.
 *
 * @param statistics The statistics to display.
 */
void StatisticsModal::setStatistics(WordleStatistics::Statistics statistics) {
    headerStatLabels[0]->setText(QString::number(statistics.gamesPlayed));
    headerStatLabels[1]->setText(QString::number(statistics.winPercentage));
    headerStatLabels[2]->setText(QString::number(statistics.currentStreak));
    headerStatLabels[3]->setText(QString::number(statistics.maxStreak));

    int minBarLength = 25;
    int maxBarLength = 200;
    int largestDistribution = 0;

    // Find the Largest Distribution Value //
    for (int i = 0; i < statistics.guessDistribution.size(); i++) {
        guessDistributionBars[i]->setText(QString::number(statistics.guessDistribution[i]));
        if (statistics.guessDistribution[i] > largestDistribution) {
            largestDistribution = statistics.guessDistribution[i];
        }
    }

    // Calculate Bar Widths Relative to the Distribution Values //
    for (int i = 0; i < statistics.guessDistribution.size(); i++) {
        int barLength;
        if (largestDistribution == 0) {
            barLength = minBarLength;
        }
        else {
            float ratio = static_cast<float>(statistics.guessDistribution[i]) / largestDistribution;
            barLength = std::clamp(static_cast<int>(ratio * maxBarLength), minBarLength, maxBarLength);
        }
        guessDistributionBars[i]->setFixedWidth(barLength);
    }
}


/**
 * @brief Enable the ability to restart from the Statistics Modal.
 */
void StatisticsModal::enableRestart() {
    modalLayout->addLayout(playAgainButtonLayout);
    modalContainer->adjustSize();
}


/**
 * @brief Disable the ability to restart from the Statistics Modal.
 */
void StatisticsModal::disableRestart() {
    modalLayout->removeItem(playAgainButtonLayout);
    modalContainer->adjustSize();
}
