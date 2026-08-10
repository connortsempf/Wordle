/**
 * @file PopUpMessenger.cpp
 * @brief Pop Up Messenger Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the PopUpMessenger class, which
 * represents a single pop up messenger in the Wordle UI.
 */


#include "View/Components/PopUpMessenger.hpp"


/**
 * @brief Constructs a PopUpMessenger object.
 *
 * @param parent The parent QWidget.
 */
PopUpMessenger::PopUpMessenger(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
}


/**
 * @brief Set the message of the pop up.
 *
 * @param message The message text displayed by the pop up.
 */
void PopUpMessenger::addMessage(const QString& message) {
    // Construct New Label //
    QLabel* label = new QLabel(this);
    label->setText(message);
    label->setFixedHeight(42);
    label->setAlignment(Qt::AlignCenter);
    label->setContentsMargins(15, 0, 15, 0);
    label->setFont(QFont("Helvetica Neue", 11, QFont::Bold));
    const auto& theme = ColorTheme::instance().getTheme();
    QColor textColor = theme.popUpMessage.text;
    QColor backgroundColor = theme.popUpMessage.background;
    QString style = QString("color: rgb(%1, %2, %3); background: rgb(%4, %5, %6); border-radius: 4px;")
        .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
        .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    label->setStyleSheet(style);


    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, label, [label]() {
        const auto& theme = ColorTheme::instance().getTheme();
        QColor textColor = theme.popUpMessage.text;
        QColor backgroundColor = theme.popUpMessage.background;

        QString style = QString("color: rgb(%1, %2, %3); background: rgb(%4, %5, %6); border-radius: 4px;")
            .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue())
            .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
        label->setStyleSheet(style);
    });


    label->adjustSize();
    popUps.push_front(label);
    positionPopUps();
    label->show();
    QTimer::singleShot(1200, this, [this, label]() { fadeOutPopUp(label); });
}


/**
 * @brief Visually fade out a pop up message.
 */
void PopUpMessenger::fadeOutPopUp(QLabel* popUp) {
    // Fade Effect //
    QGraphicsOpacityEffect* fadeEffect = new QGraphicsOpacityEffect(popUp);
    popUp->setGraphicsEffect(fadeEffect);

    // Fade Animation //
    QPropertyAnimation* fadeAnimation = new QPropertyAnimation(fadeEffect, "opacity");
    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this, popUp]() {
        popUps.removeOne(popUp);
        popUp->deleteLater();
    });
    fadeAnimation->setDuration(200);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);
    fadeAnimation->start();
}


/**
 * @brief Resize event triggered function.
 */
void PopUpMessenger::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    positionPopUps();
}


/**
 * @brief Position the pop ups within the messenger.
 */
void PopUpMessenger::positionPopUps() {
    int startingYPos = 68;
    int spacing = 20;

    for (int i = 0; i < popUps.size(); i++) {
        QLabel* label = popUps[i];
        label->move((width() - label->width()) / 2, startingYPos + (i * label->height()) + (i * spacing));
    }
}
