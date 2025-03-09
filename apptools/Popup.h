#ifndef POPUP_H
#define POPUP_H

#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>


namespace apptools {

    class PopUp : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(float popupOpacity READ getPopupOpacity WRITE setPopupOpacity)

        void setPopupOpacity(const float opacity);
        float getPopupOpacity() const;

    public:

        explicit PopUp(QWidget* parent = nullptr);

        void mouseDoubleClickEvent(QMouseEvent* event);

    signals:
        void requestNotificationsViewFromCpp();


    public slots:
        void setPopupText(const QString& text, const QString& color, const int seconds);
        void show();


    private slots:
        void hideAnimation();
        void hide();

    protected:
        void paintEvent(QPaintEvent* event);

    private:
        float _popupOpacity; //Out of style because of Qt funny macroses

        QLabel _label;
        QGridLayout _layout;
        QPropertyAnimation _animation;
        QTimer _timer;

        QColor _currentColor;
        int _secondsToStay;
    };
}


#endif // POPUP_H
