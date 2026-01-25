/**
 * @file PopUpMessenger.hpp
 * @brief Pop Up Messenger Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the PopUpMessenger class, which
 * represents a single Pop Up Messenger in the Wordle UI.
 */


#pragma once
#include <QString>
#include <QList>
#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Represents a single pop up messenger in the Wordle UI.
 */
class PopUpMessenger : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a PopUpMessenger object.
         *
         * @param parent The parent QWidget.
         */
        explicit PopUpMessenger(QWidget *parent = nullptr);

        /**
         * @brief Set the message text for a new pop up.
         *
         * @param message The message text displayed by the next pop up.
         */
        void addMessage(const QString& message);

    protected:
        /**
         * @brief Resize event triggered function.
         */
        void resizeEvent(QResizeEvent* event) override;

    private:
        /**
         * @brief Container of message pop ups.
         */
        QList<QLabel*> popUps;

    private:
        /**
         * @brief Position the pop ups within the messenger.
         */
        void positionPopUps();

        /**
         * @brief Visually fade out a pop up message.
         *
         * @param popUp The pop up message to fade out.
         */
        void fadeOutPopUp(QLabel* popUp);
};
