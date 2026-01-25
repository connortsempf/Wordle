/**
 * @file ColorTheme.cpp
 * @brief Wordle Color Theme Class Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the application ColorTheme class, which
 * devises the outline for the application color themes for the UI and its components.
 */


#include "View/Themes/ColorTheme.hpp"


/**
 * @brief
 */
const QColor ColorTheme::absentDark = QColor(58, 58, 60);

/**
 * @brief
 */
const QColor ColorTheme::absentLight = QColor(120, 124, 126);

/**
 * @brief
 */
const QColor ColorTheme::presentRegularContrast = QColor(181, 159, 59);

/**
 * @brief
 */
const QColor ColorTheme::presentHighContrast = QColor(133, 192, 249);

/**
 * @brief
 */
const QColor ColorTheme::correctRegularContrast = QColor(83, 141, 78);

/**
 * @brief
 */
const QColor ColorTheme::correctHighContrast = QColor(245, 121, 58);

/**
 * @brief The active theme.
 */
ColorTheme::Themes ColorTheme::activeTheme = ColorTheme::Themes::DARK;

/**
 * @brief The active contrast mode.
 */
ColorTheme::ContrastMode ColorTheme::activeContrastMode = ColorTheme::ContrastMode::REGULAR_CONTRAST;

/**
 * @brief The dark theme (defaulted to regular contrast mode).
 */
ColorTheme::Theme ColorTheme::darkTheme = {
    .text = {
        .glyphBackground = QColor(255, 255, 255)
    },
    .texture = {
        .glyphBackground = QColor(255, 255, 255)
    },
    .indicator = {
        .absent = ColorTheme::absentDark,
        .present = ColorTheme::presentRegularContrast,
        .correct = ColorTheme::correctRegularContrast
    },
    .background = {
        .background = QColor(18, 18, 19)
    },
    .modal = {
        .background = QColor(18, 18, 19),
        .border = QColor(26, 26, 27)
    },
    .guessBoard = {
        .tileTextUncommitted = QColor(255, 255, 255),
        .tileTextCommitted = QColor(255, 255, 255),
        .tileBorderEmpty = QColor(58, 58, 60),
        .tileBorderFilled = QColor(86, 87, 88)
    },
    .keyboard = {
        .keyBackground = QColor(129, 131, 132),
        .keyTextUncommitted = QColor(255, 255, 255),
        .keyTextCommitted = QColor(255, 255, 255)
    },
    .headerButton = {
        .dynamicBackground = QColor(54, 54, 54)
    },
    .settingsSwitch = {
        .enabledOffTrack = QColor(86, 87, 88),
        .enabledOffThumb = QColor(255, 255, 255),
        .disabledOffTrack = QColor(52, 53, 54),
        .disabledOffThumb = QColor(137, 137, 137)
    },
    .popUpMessage = {
        .text = QColor(0, 0, 0),
        .background = QColor(255, 255, 255)
    },
    .distributionBars = {
        .background = QColor(58, 58, 60)
    },
    .divider = {
        .background = QColor(58, 58, 60)
    }
};

/**
 * @brief The light theme (defaulted to regular contrast mode).
 */
ColorTheme::Theme ColorTheme::lightTheme = {
    .text = {
        .glyphBackground = QColor(0, 0, 0)
    },
    .texture = {
        .glyphBackground = QColor(0, 0, 0)
    },
    .indicator = {
        .absent = ColorTheme::absentLight,
        .present = ColorTheme::presentRegularContrast,
        .correct = ColorTheme::correctRegularContrast
    },
    .background = {
        .background = QColor(255, 255, 255)
    },
    .modal = {
        .background = QColor(255, 255, 255),
        .border = QColor(246, 247, 248)
    },
    .guessBoard = {
        .tileTextUncommitted = QColor(0, 0, 0),
        .tileTextCommitted = QColor(255, 255, 255),
        .tileBorderEmpty = QColor(211, 214, 218),
        .tileBorderFilled = QColor(135, 138, 140)
    },
    .keyboard = {
        .keyBackground = QColor(211, 214, 218),
        .keyTextUncommitted = QColor(0, 0, 0),
        .keyTextCommitted = QColor(255, 255, 255)
    },
    .headerButton = {
        .dynamicBackground = QColor(235, 235, 235)
    },
    .settingsSwitch = {
        .enabledOffTrack = QColor(135, 138, 140),
        .enabledOffThumb = QColor(255, 255, 255),
        .disabledOffTrack = QColor(195, 196, 197),
        .disabledOffThumb = QColor(255, 255, 255)
    },
    .popUpMessage = {
        .text = QColor(255, 255, 255),
        .background = QColor(0, 0, 0)
    },
    .distributionBars = {
        .background = QColor(120, 124, 126)
    },
    .divider = {
        .background = QColor(211, 214, 218)
    }
};


/**
 * @brief .
 *
 * @return .
 */
ColorTheme& ColorTheme::instance() {
    static ColorTheme instance;
    return instance;
}


/**
 * @brief Get the active theme.
 */
const ColorTheme::Theme& ColorTheme::getTheme() {
    if (activeTheme == ColorTheme::Themes::DARK) return darkTheme;
    if (activeTheme == ColorTheme::Themes::LIGHT) return lightTheme;
    return darkTheme;
}


/**
 * @brief Set the active theme.
 */
void ColorTheme::setTheme(ColorTheme::Themes theme) {
    activeTheme = theme;
    emit themeChanged();
}


/**
 * @brief .
 *
 * @param contrastMode .
 */
void ColorTheme::setContrastMode(ColorTheme::ContrastMode contrastMode) {
    activeContrastMode = contrastMode;
    if (contrastMode == ColorTheme::ContrastMode::REGULAR_CONTRAST) {
        darkTheme.indicator.present = ColorTheme::presentRegularContrast;
        darkTheme.indicator.correct = ColorTheme::correctRegularContrast;
        lightTheme.indicator.present = ColorTheme::presentRegularContrast;
        lightTheme.indicator.correct = ColorTheme::correctRegularContrast;
    }
    else if (contrastMode == ColorTheme::ContrastMode::HIGH_CONTRAST) {
        darkTheme.indicator.present = ColorTheme::presentHighContrast;
        darkTheme.indicator.correct = ColorTheme::correctHighContrast;
        lightTheme.indicator.present = ColorTheme::presentHighContrast;
        lightTheme.indicator.correct = ColorTheme::correctHighContrast;
    }
    emit themeChanged();
}
