/**
 * @file Switch.cpp
 * @brief Switch Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the Switch class, which
 * represents a single switch in the Wordle UI.
 */


#include "View/Components/Switch.hpp"


/**
 * @brief Constructs a Switch object.
 *
 * @param parent The parent QWidget.
 */
Switch::Switch(QWidget *parent) : QCheckBox(parent) {
    // Construct Track //
    setFixedSize(36, 24);
    setContentsMargins(2, 2, 2, 2);
    setCursor(Qt::PointingHandCursor);

    // Construct Thumb //
    thumb = new QWidget(this);
    thumb->setFixedSize(16, 16);
    thumb->setStyleSheet("background: white; border-radius: 8px; border: none;");
    thumb->move(3, 4);

    // Connect the Thumb to Its Animation //
    connect(this, &QCheckBox::toggled, this, &Switch::animateSwitching);
    connect(this, &QCheckBox::clicked, this, [this]() { if (!this->isCheckable()) { emit disabledSwitchPressed(); } });
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this]() {
        const auto& theme = ColorTheme::instance().getTheme();

        // Style the Thumb //
        QColor thumbColor;
        if (this->isCheckable()) {
            if (this->isChecked()) thumbColor = QColor(255, 255, 255);
            else if (!this->isChecked()) thumbColor = theme.settingsSwitch.enabledOffThumb;
        }
        else if (!this->isCheckable()) {
            if (this->isChecked()) thumbColor = QColor(255, 255, 255);
            else if (!this->isChecked()) thumbColor = theme.settingsSwitch.disabledOffThumb;
        }
        QString thumbStyle = QString("border-radius: 8px; border: none; background: rgb(%1, %2, %3); ")
            .arg(thumbColor.red()).arg(thumbColor.green()).arg(thumbColor.blue());
        this->thumb->setStyleSheet(thumbStyle);

        // Style the Track //
        QColor trackOnColor = theme.indicator.correct;
        QColor trackEnabledOffColor = theme.settingsSwitch.enabledOffTrack;
        QColor trackDisabledOffColor = theme.settingsSwitch.disabledOffTrack;
        QString trackStyle = QString(
            "QCheckBox { background: transparent; border: none; padding: 0px; }"
            "QCheckBox::indicator { width: 32px; height: 20px; border-radius: 10px; border: none; }"
            "QCheckBox::indicator:disabled:checked { background: rgb(%1, %2, %3); }"
            "QCheckBox::indicator:disabled:unchecked { background: rgb(%4, %5, %6); }"
            "QCheckBox::indicator:checked { background: rgb(%7, %8, %9); }"
            "QCheckBox::indicator:unchecked { background: rgb(%10, %11, %12); }"
        ).arg(trackOnColor.red()).arg(trackOnColor.green()).arg(trackOnColor.blue())
         .arg(trackDisabledOffColor.red()).arg(trackDisabledOffColor.green()).arg(trackDisabledOffColor.blue())
         .arg(trackOnColor.red()).arg(trackOnColor.green()).arg(trackOnColor.blue())
         .arg(trackEnabledOffColor.red()).arg(trackEnabledOffColor.green()).arg(trackEnabledOffColor.blue());
        this->setStyleSheet(trackStyle);
    });
}


/**
 * @brief Animates the appending of a letter to the tile.
 */
void Switch::animateSwitching() {
    QPropertyAnimation* anim = new QPropertyAnimation(thumb, "pos");
    anim->setDuration(200);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    anim->setStartValue(thumb->pos());
    anim->setEndValue(this->isChecked() ? QPoint(13, 4) : QPoint(3, 4));
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
