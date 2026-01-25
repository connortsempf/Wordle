/**
 * @file ColorTheme.hpp
 * @brief Wordle Color Theme Class Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the application ColorTheme class, which
 * devises the outline for the application color themes for the UI and its components.
 */


#pragma once
#include <QColor>
#include <QObject>


/**
 * @brief Color Theme class.
 */
class ColorTheme : public QObject {

    Q_OBJECT

    public:
        /**
         * @brief Theme options
         */
        enum class Themes { DARK, LIGHT };

        /**
         * @brief Contrast mode options
         */
        enum class ContrastMode { REGULAR_CONTRAST, HIGH_CONTRAST };

    public:
        /**
         * @brief
         */
        struct Theme {
            /**
             * @brief
             */
            struct Text { QColor glyphBackground; };

            /**
             * @brief
             */
            struct Texture { QColor glyphBackground; };

            /**
             * @brief .
             */
            struct Indicator { QColor absent; QColor present; QColor correct; };

            /**
             * @brief
             */
            struct Background { QColor background; };

            /**
             * @brief
             */
            struct Modal { QColor background; QColor border; };

            /**
             * @brief
             */
            struct GuessBoard { QColor tileTextUncommitted; QColor tileTextCommitted; QColor tileBorderEmpty; QColor tileBorderFilled; };

            /**
             * @brief
             */
            struct Keyboard { QColor keyBackground; QColor keyTextUncommitted; QColor keyTextCommitted; };

            /**
             * @brief
             */
            struct HeaderButton { QColor dynamicBackground; };

            /**
             * @brief
             */
            struct SettingsSwitch { QColor enabledOffTrack; QColor enabledOffThumb; QColor disabledOffTrack; QColor disabledOffThumb; };

            /**
             * @brief
             */
            struct PopUpMessage { QColor text; QColor background; };

            /**
             * @brief
             */
            struct DistributionBars { QColor background; };

            /**
             * @brief
             */
            struct Divider { QColor background; };

            Text text;
            Texture texture;
            Indicator indicator;
            Background background;
            Modal modal;
            GuessBoard guessBoard;
            Keyboard keyboard;
            HeaderButton headerButton;
            SettingsSwitch settingsSwitch;
            PopUpMessage popUpMessage;
            DistributionBars distributionBars;
            Divider divider;
        };

    public:
        /**
         * @brief .
         *
         * @return .
         */
        static ColorTheme& instance();

        /**
         * @brief Get the active theme.
         *
         * @return .
         */
        const Theme& getTheme();

        /**
         * @brief Set the active theme.
         *
         * @param theme .
         */
        void setTheme(ColorTheme::Themes theme);

        /**
         * @brief .
         *
         * @param contrastMode .
         */
        void setContrastMode(ColorTheme::ContrastMode contrastMode);

    signals:
        /**
         * @brief Signal emitted when the global theme changes.
         */
        void themeChanged();

    private:
        /**
         * @brief
         */
        static const QColor absentDark;

        /**
         * @brief
         */
        static const QColor absentLight;

        /**
         * @brief
         */
        static const QColor presentRegularContrast;

        /**
         * @brief
         */
        static const QColor presentHighContrast;

        /**
         * @brief
         */
        static const QColor correctRegularContrast;

        /**
         * @brief
         */
        static const QColor correctHighContrast;

        /**
         * @brief The active theme.
         */
        static Themes activeTheme;

        /**
         * @brief The active contrast mode.
         */
        static ContrastMode activeContrastMode;

        /**
         * @brief The dark theme.
         */
        static Theme darkTheme;

        /**
         * @brief The light theme.
         */
        static Theme lightTheme;

    private:
        ColorTheme(QObject* parent = nullptr) : QObject(parent) {}
        ColorTheme(const ColorTheme&) = delete;
        ColorTheme& operator=(const ColorTheme&) = delete;
};
