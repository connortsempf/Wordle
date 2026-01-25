/**
 * @file Modal.hpp
 * @brief Modal Component Definition.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the definition of the Modal class, which
 * represents the pop up overlay screen component in the Wordle UI.
 */


#pragma once
#include <QEvent>
#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include "View/Themes/ColorTheme.hpp"


/**
 * @brief Reusable Modal pop up component class.
 */
class Modal : public QWidget {

    Q_OBJECT

    public:
        /**
         * @brief Constructs a Modal object.
         *
         * @param parent The parent QWidget.
         */
        explicit Modal(QWidget *parent = nullptr);

    protected:
        /**
         * @brief The modal container widget for containing reused-modal-specific content.
         */
        QWidget* modalContainer;

        /**
         * @brief The modal container layout for containing reused-modal-specific content.
         */
        QVBoxLayout* modalLayout;

    protected:
        /**
         * @brief Overrides resize events for internal processes.
         *
         * @param event The resize event.
         */
        void resizeEvent(QResizeEvent *event) override;

        /**
         * @brief Overrides show events for internal processes.
         *
         * @param event The show event.
         */
        void showEvent(QShowEvent* event) override;

    signals:
        /**
         * @brief Signal emitted when the modal desires closing.
         */
        void close();

        /**
         * @brief Signal emitted when the modal is done animating its entry.
         */
        void entryAnimationFinished();

    private:
        /**
         * @brief The background container widget.
         */
        QWidget* backgroundContainer;

    private:
        /**
         * @brief Event filter to handle mouse clicks on the background container.
         *
         * @param component The component that received the event.
         * @param event The event that was received.
         *
         * @return True if the event was handled, false otherwise.
         */
        bool eventFilter(QObject* component, QEvent* event);

        /**
         * @brief Animates the modal entry when opened.
         */
        void animateEntry();
};
