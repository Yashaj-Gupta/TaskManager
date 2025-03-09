#ifndef SCREENGLOBAL_H
#define SCREENGLOBAL_H

#include <QObject>
#include <QScreen>

namespace aim {
    class ScreenGlobal: public QObject
    {
        Q_OBJECT

    public:
        ScreenGlobal(QObject *parent = nullptr);

        Q_INVOKABLE int getScreenWidth() const;
        Q_INVOKABLE int getScreenHeight() const;

        Q_INVOKABLE double getDPI() const;

        Q_INVOKABLE double adaptXSize(const int xValue) const;
        Q_INVOKABLE double adaptYSize(const int yValue) const;

    protected:

        QScreen& screen;
    };

}


#endif // SCREENGLOBAL_H
