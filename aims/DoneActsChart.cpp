#include "DoneActsChart.h"


using namespace aim;


void DoneActsChartQML::paint(QPainter* painter){

    for (auto i = 0; i < 24; ++i){
        int x = (i+1)*hourWidth;
        painter->drawLine(x,aimHeight,x,aimHeight*2);
        painter->drawText(x,aimHeight/2,QString::number(i));
    }

    QSet<QString> paintedAims;

    for (auto i = 0; i < lines.size(); ++i){
        int aimIndex = indexation[ lines[i].aimId ];
        switch (aimIndex % 7){
            case 0: painter->setPen(Qt::red); break;
            case 1: painter->setPen(Qt::yellow); break;
            case 2: painter->setPen(Qt::green); break;
            case 3: painter->setPen(Qt::white); break;
            case 4: painter->setPen(Qt::magenta); break;
            case 5: painter->setPen(Qt::cyan); break;
            case 6: painter->setPen(Qt::blue); break;
        }

        const int y = (aimHeight*3) + aimHeight*aimIndex; //should insure there is such thing
        const int x1 = lines[i].x1;
        const int x2 = lines[i].x2;
        painter->drawLine(x1,y,x2,y);

        if (paintedAims.contains(lines[i].aimId)==false){
            painter->drawText(x1,y-aimHeight/2,lines[i].aimName);
            paintedAims.insert(lines[i].aimId);
        }

        painter->drawEllipse(x1-5,y-2,5,5);
        painter->drawEllipse(x2+1,y-2,5,5);
    }
}


int getHourPart(const QString time) {
    return time.midRef(0,time.indexOf(":")).toInt();
}


int getTotalSecondsButNoHours(const QString time){
    const QString withourHours = time.mid(time.indexOf(":")+1);
    const QString minutes = withourHours.mid(0, withourHours.indexOf(":"));
    const QString seconds = withourHours.mid(withourHours.indexOf(":") +1);
    const int totalSeconds = minutes.toInt()* 60 + seconds.toInt();
    return totalSeconds;
}


void DoneActsChartQML::setSource(const QVariantList actsList){

    indexation.clear();
    lines.clear();
    int counting = 0;

    for (auto i = 0; i < actsList.size(); ++i){
        QStringList oneLine = actsList[i].toStringList();

        if (oneLine[3] == "start")
            continue;
        if (indexation.contains(oneLine[2]) == false){
            indexation.insert(oneLine[2],counting);
            ++counting;
        }

        const QString timePart = oneLine[4].mid(oneLine[4].indexOf("T")+1);
        const int hourPart = getHourPart(timePart);
        const int restSeconds = getTotalSecondsButNoHours(timePart);
        const int someX = (hourPart + 1)*hourWidth;
        const int shift = (restSeconds*hourWidth)/3600; //60*60 seconds in hour
        const int backShift = (oneLine[5].toInt()*hourWidth)/3600;
        const int x2 = someX + shift;
        const int x1 = x2 - backShift;

        DoneActsChartLine line;
        line.x1 = x1; line.x2 = x2;
        line.aimId = oneLine[2];
        line.aimName = oneLine[1];
        lines << line;
    }
    update();
}


int DoneActsChartQML::getFirstRecordX() const{
    if (lines.size() == 0)
        return 0;
    return lines[0].x1;
}
