/**
 * @file Modal.cpp
 * @brief Modal Component Implementation.
 * @date 2026-01-21
 * @version 1.0.0
 *
 * This file contains the implementation of the Modal class, which
 * represents the pop up overlay screen component in the Wordle UI.
 */


#include "View/Components/Modal.hpp"


/**
 * @brief Constructs a Modal object.
 *
 * @param parent The parent QWidget.
 */
Modal::Modal(QWidget *parent) : QWidget(parent) {
    // Background Container //
    backgroundContainer = new QWidget(this);
    backgroundContainer->setContentsMargins(0, 0, 0, 0);
    backgroundContainer->setStyleSheet("background-color: rgba(0, 0, 0, 128); border: none;");
    backgroundContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    backgroundContainer->installEventFilter(this);

    // Modal Container //
    modalContainer = new QWidget(backgroundContainer);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setXOffset(0);
    shadow->setYOffset(0);
    shadow->setColor(QColor(0, 0, 0, 100));
    modalContainer->setAttribute(Qt::WA_NoMousePropagation, true);
    modalContainer->setFixedWidth(450);
    modalContainer->setGraphicsEffect(shadow);

    // X Button //
    QPushButton* closeButton = new QPushButton(modalContainer);
    closeButton->setFixedSize(34, 34);
    closeButton->setIcon(QIcon(":/images/assets/images/x-icon.png"));
    closeButton->setIconSize(QSize(34, 34));
    closeButton->setStyleSheet("border-radius: 0px; border: none;");
    connect(closeButton, &QPushButton::clicked, this, &Modal::close);
    closeButton->setCursor(Qt::PointingHandCursor);
    QHBoxLayout* closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->setContentsMargins(0, 0, 0, 0);
    closeButtonLayout->setAlignment(Qt::AlignRight);
    closeButtonLayout->addStretch();
    closeButtonLayout->addWidget(closeButton);

    // Modal Container Layout //
    modalLayout = new QVBoxLayout(modalContainer);
    modalLayout->setContentsMargins(15, 15, 15, 15);
    modalLayout->setAlignment(Qt::AlignTop);
    modalLayout->addLayout(closeButtonLayout);
    // modalContainer->adjustSize();

    // Background Container Layout //
    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundContainer);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->setAlignment(Qt::AlignCenter);

    // Main Layout //
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(backgroundContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Set Theme //
    connect(&ColorTheme::instance(), &ColorTheme::themeChanged, this, [this, closeButton]() {
        const auto& theme = ColorTheme::instance().getTheme();
        // Apply Modal Background and Border Colors //
        QColor backgroundColor = theme.modal.background;
        QColor borderColor = theme.modal.border;
        QString style = QString("background-color: rgb(%1, %2, %3); border: 1px solid rgb(%4, %5, %6); border-radius: 8px; ")
            .arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue()).arg(borderColor.red()).arg(borderColor.green()).arg(borderColor.blue());
        modalContainer->setStyleSheet(style);

        // Recolor the Close Button //
        QColor iconColor = theme.texture.glyphBackground;
        QPixmap closeButtonOriginal(":/images/assets/images/x-icon.png");
        QPixmap closeButtonPainted = closeButtonOriginal;
        closeButtonPainted.fill(Qt::transparent);
        QPainter painter(&closeButtonPainted);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawPixmap(0, 0, closeButtonOriginal);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.fillRect(closeButtonPainted.rect(), iconColor);
        painter.end();
        closeButton->setIcon(QIcon(closeButtonPainted));
    });

    setGeometry(0, 0, parent->width(), parent->height());
    setLayout(mainLayout);
}


/**
 * @brief Overrides resize events for internal processes.
 *
 * @param event The resize event.
 */
void Modal::resizeEvent(QResizeEvent *event) {
    // Reposition the Modal Manually //
    int backgroundXPos = backgroundContainer->x();
    int backgroundYPos = backgroundContainer->y();
    int backgroundWidth = backgroundContainer->width();
    int backgroundHeight = backgroundContainer->height();
    int containerWidth = modalContainer->width();
    int containerHeight = modalContainer->sizeHint().height();
    modalContainer->move(
        backgroundXPos + (backgroundWidth / 2) - (containerWidth / 2),
        backgroundYPos + (backgroundHeight / 2) - (containerHeight / 2)
    );
}


/**
 * @brief Overrides show events for internal processes.
 *
 * @param event The show event.
 */
void Modal::showEvent(QShowEvent* event) {
    // Call Base Class Implementation //
    QWidget::showEvent(event);
    animateEntry();
}


/**
 * @brief Event filter to handle mouse clicks on the background container.
 *
 * @param component The component that received the event.
 * @param event The event that was received.
 *
 * @return True if the event was handled, false otherwise.
 */
bool Modal::eventFilter(QObject* component, QEvent* event) {
    // Only Handle Mouse Clicks on the Background Container //
    if (component == backgroundContainer && event->type() == QEvent::MouseButtonPress) {
        emit close();
        return true;
    }
    // Pass All Other Events to Base Class //
    return QWidget::eventFilter(component, event);
}


/**
 * @brief Animates the modal entry when opened.
 */
void Modal::animateEntry() {
    // Rise Animation Positioning //
    int backgroundXPos = backgroundContainer->x();
    int backgroundYPos = backgroundContainer->y();
    int backgroundWidth = backgroundContainer->width();
    int backgroundHeight = backgroundContainer->height();
    int containerWidth = modalContainer->width();
    int containerHeight = modalContainer->sizeHint().height();
    int targetXPos = backgroundXPos + (backgroundWidth / 2) - (containerWidth / 2);
    int targetYPos = backgroundYPos + (backgroundHeight / 2) - (containerHeight / 2);
    modalContainer->move(targetXPos, targetYPos + 100);

    // Rise Animation //
    QPropertyAnimation* riseAnimation = new QPropertyAnimation(modalContainer, "pos");
    riseAnimation->setDuration(200);
    riseAnimation->setStartValue(QPoint(targetXPos, modalContainer->y()));
    riseAnimation->setEndValue(QPoint(targetXPos, targetYPos));
    riseAnimation->setEasingCurve(QEasingCurve::OutCubic);

    // Create Opacity Effect for Fade Animation //
    QGraphicsOpacityEffect* fadeEffect = new QGraphicsOpacityEffect(modalContainer);
    modalContainer->setGraphicsEffect(fadeEffect);

    // Opacity Animation //
    QPropertyAnimation* fadeAnimation = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnimation->setDuration(200);
    fadeAnimation->setStartValue(0.0);
    fadeAnimation->setEndValue(1.0);
    fadeAnimation->setEasingCurve(QEasingCurve::InQuad);

    // Run Rise and Fade Animation in Parallel //
    QParallelAnimationGroup* animationGroup = new QParallelAnimationGroup(this);
    animationGroup->addAnimation(riseAnimation);
    animationGroup->addAnimation(fadeAnimation);
    connect(animationGroup, &QParallelAnimationGroup::finished, this, &Modal::entryAnimationFinished);
    animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
}
