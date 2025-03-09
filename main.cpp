//#include <QApplication>
#include <QtGui>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>

#include <QQuickStyle>


#include "apptools/TreeModel.h"
#include "apptools/SystemTray.h"
#include "apptools/Popup.h"
#include "apptools/LogHandler.h"
#include "apptools/Clipboard.h"
#include "aims/AimBase.h"
#include "aims/UserSettings.h"
#include "aims/ScreenGlobal.h"
#include "aims/AimNotifications.h"
#include "aims/RunningAims.h"
#include "aims/UserToken.h"
#include "aims/DoneActsChart.h"

#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#include <QAndroidService>
#endif

using namespace aim;
using namespace apptools;

int main(int argc, char *argv[])
{
    #ifdef Q_OS_ANDROID
    QAndroidService service(argc, argv);
    #endif
    QApplication app(argc, argv);

    ScreenGlobal screen;
    LocalSqlBase localBase(&app);
    localBase.createTablesIfNeeded();
    UserSettings userSettings(&app);
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    context->setContextProperty("localBase",&localBase);
    context->setContextProperty("userSettings",&userSettings);
    context->setContextProperty("screenGlobal",&screen);
    context->setContextProperty("treeModel",&localBase.aimsTree);
    context->setContextProperty("tagTree",&localBase.tagsTree);

    SystemTray tray(&app);
    context->setContextProperty("systemTray",&tray);
    PopUp popUp;
    context->setContextProperty("popUpItem",&popUp);

    AimNotifications notifications(localBase, popUp, &app);
    notifications.startWatchDog(30);
    engine.rootContext()->setContextProperty("androidNotify",&notifications.androidNotification);

    RunningAims runningAims(localBase, &app);
    context->setContextProperty("runningAims",&runningAims);

    LogHandler::getInst().setFilename("log.txt");

    UserToken token;
    engine.rootContext()->setContextProperty("userToken",&token);

    Clipboard clip;
    engine.rootContext()->setContextProperty("clipboard",&clip);

    qmlRegisterType<ConsoleLogQML>("aim.tasks",1,0,"ConsoleLog");
    qmlRegisterType<DoneActsChartQML>("aim.tasks",1,0,"DoneActsChart");

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    return app.exec();
}


