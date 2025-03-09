#ifndef DONEACTSCHART_H
#define DONEACTSCHART_H

#include <QQuickPaintedItem>
#include <QPainter>


namespace aim {

    struct DoneActsChartLine{
        int x1;
        int x2;
        QString aimId;
        QString aimName;
    };


    class DoneActsChartQML : public QQuickPaintedItem
    {
        Q_OBJECT
    public:

        DoneActsChartQML(): hourWidth(40), aimHeight(40) {}

        Q_INVOKABLE void setHourWidth(const int newWidth) { hourWidth = newWidth; }
        Q_INVOKABLE int getTotalAims() const { return indexation.size(); }
        Q_INVOKABLE void setAimHeight(const int newHeight) { aimHeight = newHeight; }

        void paint(QPainter* painter);

        Q_INVOKABLE void setSource(const QVariantList actsList);
        Q_INVOKABLE int getFirstRecordX() const;

    protected:

        QMap< QString, int > indexation;
        QList< DoneActsChartLine > lines;

        int hourWidth;
        int aimHeight;
    };
}

#endif // DONEACTSCHART_H
