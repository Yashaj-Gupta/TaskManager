#include "picturemodel.h"

#include <QQmlEngine>

PictureForQml::PictureForQml(QObject *parent) :
    QObject(parent),pictureIsEmpty(true)
{
}

QObject *PictureForQml::getPixmapContainer() const
{
    PixmapContainer * pc = new PixmapContainer();

    if (pictureIsEmpty == false)
        pc->pixmap.loadFromData(lastLoadedPicture);
    else
    {
        pc->pixmap.load(":/icons/nophoto.jpg"); //can load image of unknown
    }

    Q_ASSERT(!pc->pixmap.isNull());
    QQmlEngine::setObjectOwnership(pc, QQmlEngine::JavaScriptOwnership);
    return pc;
}

PixmapImage::PixmapImage(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
}

void PixmapImage::setImage(QObject *pixmapContainer)
{
    PixmapContainer *pc = qobject_cast<PixmapContainer*>(pixmapContainer);
    Q_ASSERT(pc);
    m_pixmapContainer.pixmap = pc->pixmap; //default value of height
    update();
}

void PixmapImage::paint(QPainter *painter)
{
    painter->drawPixmap(0, 0, width(), height(), m_pixmapContainer.pixmap);
}
