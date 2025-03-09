#include "SystemTray.h"

#include <QMenu>
#include <QSystemTrayIcon>

using namespace apptools;

SystemTray::SystemTray(QObject *parent) : QObject(parent), trayIcon(0) {

    QAction * viewWindow = new QAction("Show", this);
    QAction * addAction = new QAction("Add", this);
    QAction * quitAction = new QAction("Quit", this);
    connect(viewWindow, &QAction::triggered, this, &SystemTray::signalShow);
    connect(quitAction, &QAction::triggered, this, &SystemTray::signalQuit);
    connect(addAction, &QAction::triggered, this, &SystemTray::signalAdd);

    QMenu *trayIconMenu = new QMenu();
    trayIconMenu->addAction(viewWindow);
    trayIconMenu->addAction(addAction);
    trayIconMenu->addAction(quitAction);

    trayIcon.setContextMenu(trayIconMenu);
    trayIcon.setIcon(QIcon(":/icons/tray.png"));
    trayIcon.setToolTip("Aim tasktracker");
    trayIcon.show();
    trayIcon.setToolTip("Wavesplat");

    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}


SystemTray::~SystemTray(){
    trayIcon.hide();
}


void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason){
    case QSystemTrayIcon::Trigger:
        emit signalIconActivated();
        break;
    default:
        break;
    }
}


void SystemTray::hideIconTray() {
    trayIcon.hide();
}


void SystemTray::showNotification(const QString title, const QString text, const int seconds) {
    trayIcon.showMessage(title, text, QSystemTrayIcon::Information, seconds * 1000);
}


void SystemTray::showWarning(const QString title, const QString text, const int seconds) {
    trayIcon.showMessage(title, text, QSystemTrayIcon::Warning, seconds * 1000);
}


void SystemTray::showError(const QString title, const QString text, const int seconds) {
    trayIcon.showMessage(title, text, QSystemTrayIcon::Critical, seconds * 1000);
}

