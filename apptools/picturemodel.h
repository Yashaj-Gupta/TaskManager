#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include <QObject>
#include <QPixmap>

#include <QQuickPaintedItem>
#include <QPainter>

#include <QByteArray>

///HEY DON"T LIKE THAT WE HAVE CONNECTION TO BASE HERE
// GOOD TO HAVE IT SEPPARATE class - and current one - nice to make abstract parent - mm!

//#include "sqlbaseaccess.h"

class PixmapContainer : public QObject
{
    Q_OBJECT
public:
    explicit PixmapContainer(QObject *parent = 0): QObject(parent) {}
    QPixmap pixmap;
};

class PictureForQml : public QObject
{
    Q_OBJECT
public:
    explicit PictureForQml(QObject *parent = 0);

    Q_INVOKABLE QObject* getPixmapContainer() const;

protected:
    bool pictureIsEmpty;

     QByteArray lastLoadedPicture;
};



class PixmapImage : public QQuickPaintedItem
{
    Q_OBJECT
public:
    PixmapImage(QQuickItem *parent = Q_NULLPTR);
    Q_INVOKABLE void setImage(QObject *pixmapContainer);

protected:
    virtual void paint(QPainter *painter) Q_DECL_OVERRIDE;

private:
    PixmapContainer m_pixmapContainer;
};


#endif // PICTUREMODEL_H
