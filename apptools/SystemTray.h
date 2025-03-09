#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>

namespace apptools {

    class SystemTray : public QObject
    {
        Q_OBJECT
    public:
        explicit SystemTray(QObject *parent = nullptr);
        ~SystemTray();

        Q_INVOKABLE void showNotification(const QString title, const QString text, const int seconds);
        Q_INVOKABLE void showWarning(const QString title, const QString text, const int seconds);
        Q_INVOKABLE void showError(const QString title, const QString text, const int seconds);

        void setTrayIcon(const QSystemTrayIcon &newTrayIcon);

    signals:
        void signalIconActivated();
        void signalShow();
        void signalQuit();
        void signalAdd();

    signals:
        void requestTrayActionFromCpp();

    private slots:
        void iconActivated(QSystemTrayIcon::ActivationReason reason);

    public slots:
        void hideIconTray();

    private:
        QSystemTrayIcon trayIcon;
    };
}


#endif // SYSTEMTRAY_H
