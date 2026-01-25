/**
 * @file SettingsModal.cpp
 * @brief Settings Modal Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the SettingsModal class, which
 * represents the settings modal in the Wordle UI.
 */


#include "View/Modals/SettingsModal.hpp"


/**
 * @brief Constructs a SettingsModal object.
 *
 * @param parent The parent QWidget.
 */
SettingsModal::SettingsModal(QWidget *parent) : Modal(parent) {
    // Title Text //
    QLabel *title = new QLabel("Settings", modalContainer);
    title->setAlignment(Qt::AlignCenter);
    title->setContentsMargins(0, 0, 0, 0);
    title->setFont(QFont("NYTKarnak", 24));
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, title]() {
        const auto& theme = ColorTheme::instance().getTheme();
        QColor textColor = theme.text.glyphBackground;
        QString textStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
        title->setStyleSheet(textStyle);
    });

    // Settings Options Layout //
    QVBoxLayout* settingsOptionsLayout = new QVBoxLayout();
    settingsOptionsLayout->setContentsMargins(0, 0, 0, 0);
    settingsOptionsLayout->setSpacing(10);
    settingsOptionsLayout->setAlignment(Qt::AlignCenter);

    // Settings Options Components //
    std::vector<std::string> optionTitles = { "Hard Mode", "Dark Theme", "High Contrast Mode", "Onscreen Keyboard Input Only" };
    std::vector<std::string> optionSubTitles = {
        "Any Revealed hints must be used in subsequent guesses",
        "",
        "Contrast and colorblindness improvements",
        "Ignore key input except from the onscreen keyboard. Most helpful for users using speech regonition or other assistive devices."
    };
    for (int i = 0; i < 4; i++) {
        QHBoxLayout* optionLayout = new QHBoxLayout();
        optionLayout->setContentsMargins(0, 0, 0, 0);
        optionLayout->setSpacing(0);

        QVBoxLayout* optionTextLayout = new QVBoxLayout();
        optionTextLayout->setContentsMargins(0, 0, 0, 0);
        optionTextLayout->setSpacing(2);

        QLabel* optionTitle = new QLabel(QString::fromStdString(optionTitles[i]), modalContainer);
        optionTitle->setContentsMargins(0, 0, 0, 0);
        optionTitle->setFont(QFont("Arial", 14, QFont::Normal));
        optionTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        optionTextLayout->addWidget(optionTitle);
        connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, optionTitle]() {
            const auto& theme = ColorTheme::instance().getTheme();
            QColor textColor = theme.text.glyphBackground;
            QString textStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none;").arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
            optionTitle->setStyleSheet(textStyle);
        });

        if (!optionSubTitles[i].empty()) {
            QLabel* optionSubTitle = new QLabel(QString::fromStdString(optionSubTitles[i]), modalContainer);
            optionSubTitle->setWordWrap(true);
            optionSubTitle->setContentsMargins(0, 0, 0, 0);
            optionSubTitle->setFont(QFont("Arial", 10, QFont::Normal));
            optionSubTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            optionTextLayout->addWidget(optionSubTitle);
            connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, optionSubTitle]() {
                const auto& theme = ColorTheme::instance().getTheme();
                QColor textColor = theme.text.glyphBackground;
                QString textStyle = QString("color: rgb(%1, %2, %3); border-radius: none; border: none; padding-right: 20px")
                    .arg(textColor.red()).arg(textColor.green()).arg(textColor.blue());
                optionSubTitle->setStyleSheet(textStyle);
            });
        }

        settingsSwitches[i] = new Switch(modalContainer);
        Switch* optionSwitch = settingsSwitches[i];
        optionLayout->addLayout(optionTextLayout);
        optionLayout->addWidget(optionSwitch);
        connect(optionSwitch, &QCheckBox::toggled, this, [this, optionSwitch, i]() {
            if      (i == 0) optionSwitch->isChecked() ? emit enableHardMode() : emit disableHardMode();
            else if (i == 1) optionSwitch->isChecked() ? emit enableDarkTheme() : emit disableDarkTheme();
            else if (i == 2) optionSwitch->isChecked() ? emit enableHighContrastMode() : emit disableHighContrastMode();
            else if (i == 3) optionSwitch->isChecked() ? emit enableOnscreenKeyboardInputOnly() : emit disableOnscreenKeyboardInputOnly();
        });

        settingsOptionsLayout->addLayout(optionLayout);

        if (i < 3) {
            QFrame* divider = new QFrame(modalContainer);
            divider->setFixedHeight(1);
            divider->setFrameShape(QFrame::HLine);
            divider->setContentsMargins(0, 0, 0, 0);
            divider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            settingsOptionsLayout->addWidget(divider);
            connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, divider]() {
                const auto& theme = ColorTheme::instance().getTheme();
                QColor dividerColor = theme.divider.background;
                QString dividerStyle = QString("background-color: rgb(%1, %2, %3); border: none;").arg(dividerColor.red()).arg(dividerColor.green()).arg(dividerColor.blue());
                divider->setStyleSheet(dividerStyle);
            });
        }
    }

    // Add to Modal Container Layout //
    modalLayout->addWidget(title);
    modalLayout->addSpacing(10);
    modalLayout->addLayout(settingsOptionsLayout);
    modalContainer->adjustSize();
    connect(settingsSwitches[0], &Switch::disabledSwitchPressed, this, &SettingsModal::disabledSwitchPressed);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void SettingsModal::setHardModeSwitch(bool isChecked) {
    Qt::CheckState checkState;
    if (isChecked) checkState = Qt::CheckState::Checked;
    else if (!isChecked) checkState = Qt::CheckState::Unchecked;
    settingsSwitches[0]->setCheckState(checkState);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void SettingsModal::setDarkThemeSwitch(bool isChecked) {
    Qt::CheckState checkState;
    if (isChecked) checkState = Qt::CheckState::Checked;
    else if (!isChecked) checkState = Qt::CheckState::Unchecked;
    settingsSwitches[1]->setCheckState(checkState);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void SettingsModal::setHighContrastModeSwitch(bool isChecked) {
    Qt::CheckState checkState;
    if (isChecked) checkState = Qt::CheckState::Checked;
    else if (!isChecked) checkState = Qt::CheckState::Unchecked;
    settingsSwitches[2]->setCheckState(checkState);
}


/**
 * @brief .
 *
 * @param isChecked .
 */
void SettingsModal::setOnScreenKeyboardInputOnlySwitch(bool isChecked) {
    Qt::CheckState checkState;
    if (isChecked) checkState = Qt::CheckState::Checked;
    else if (!isChecked) checkState = Qt::CheckState::Unchecked;
    settingsSwitches[3]->setCheckState(checkState);
}


/**
 * @brief Enable the ability to toggle hard mode switch on and off.
 */
void SettingsModal::enableHardModeToggling() {
    settingsSwitches[0]->setCheckable(true);
}


/**
 * @brief Disable the ability to toggle hard mode switch on and off.
 */
void SettingsModal::disableHardModeToggling() {
    settingsSwitches[0]->setCheckable(false);
}
