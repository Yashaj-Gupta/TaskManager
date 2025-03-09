#include "Popup.h"

//popup headers
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QDebug>

using namespace apptools;


PopUp::PopUp(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    _animation.setTargetObject(this);
    _animation.setPropertyName("popupOpacity");
    connect(&_animation, &QAbstractAnimation::finished, this, &PopUp::hide);

    _label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    _label.setStyleSheet("QLabel { color : white; "
                        "margin-top: 6px;"
                        "margin-bottom: 6px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");

    _layout.addWidget(&_label, 0, 0);
    setLayout(&_layout);

    connect(&_timer, &QTimer::timeout, this, &PopUp::hideAnimation);
    _currentColor = QColor(0,0,0,180);
}


void PopUp::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush( _currentColor )); //QColor(0,0,0,180)
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}


void PopUp::setPopupText(const QString &text, const QString& color, const int seconds)
{
    if (color == "red") {
        _currentColor = QColor(255,10,10);
        _label.setStyleSheet("QLabel { color : white; "
                            "margin-top: 6px;"
                            "margin-bottom: 6px;"
                            "margin-left: 10px;"
                            "margin-right: 10px; }");
    }
    if (color == "gray") {
        _currentColor = QColor(90,90,90);
        _label.setStyleSheet("QLabel { color : white; "
                            "margin-top: 6px;"
                            "margin-bottom: 6px;"
                            "margin-left: 10px;"
                            "margin-right: 10px; }");
    }
    if (color == "green") {
        _currentColor = QColor(10,255,10);
        _label.setStyleSheet("QLabel { color : black; "
                            "margin-top: 6px;"
                            "margin-bottom: 6px;"
                            "margin-left: 10px;"
                            "margin-right: 10px; }");
    }
    if (color == "yellow") {
        _currentColor = QColor(255,255,10);
        _label.setStyleSheet("QLabel { color : black; "
                            "margin-top: 6px;"
                            "margin-bottom: 6px;"
                            "margin-left: 10px;"
                            "margin-right: 10px; }");
    }

    _secondsToStay = seconds;
    _label.setText(text);
    adjustSize();
}


void PopUp::show() {
    setWindowOpacity(0.0);
    _animation.setDuration(150);
    _animation.setStartValue(0.0);
    _animation.setEndValue(1.0);
    setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x(),
                QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y(),
                width(),
                height());
    QWidget::show();
    _animation.start();
    _timer.start(_secondsToStay * 1000);
}


void PopUp::mouseDoubleClickEvent([[maybe_unused]] QMouseEvent* event) {
    emit requestNotificationsViewFromCpp();
}


void PopUp::hideAnimation() {
    _timer.stop();
    _animation.setDuration(2000);
    _animation.setStartValue(1.0);
    _animation.setEndValue(0.0);
    _animation.start();
}


void PopUp::hide() {
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}


void PopUp::setPopupOpacity(const float opacity) {
    _popupOpacity = opacity;
    setWindowOpacity(opacity);
}


float PopUp::getPopupOpacity() const {
    return _popupOpacity;
}
