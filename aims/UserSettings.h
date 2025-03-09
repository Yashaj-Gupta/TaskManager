#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QObject>
#include <QList>
#include <QMap>

namespace aim {

    class UserSettings : public QObject
    {
        Q_OBJECT
    public:
        explicit UserSettings(QObject *parent = nullptr);

        Q_INVOKABLE QList<bool> getAddAimSettings() const;
        Q_INVOKABLE void setAddAimSettings(const QList<bool> newSettings);

        Q_INVOKABLE QList<bool> getViewAimSettings() const;
        Q_INVOKABLE void setViewAimSettings(const QList<bool> newSettings);

        Q_INVOKABLE bool setColor(const QString colorType, const QString colorValue);
        Q_INVOKABLE QString getColor(const QString colorType) const;

        Q_INVOKABLE quint8 getSortingOrderType() const { return _orderType; }
        Q_INVOKABLE void setSortingOrderType(quint8 newOrderType){ _orderType = newOrderType; }

        Q_INVOKABLE bool isDebugBuild() const {
            #ifdef QT_DEBUG
                return true;
            #else
                return false;
            #endif
        }

    protected:

        QMap<QString,bool> _viewAimSettings;
        QMap<QString,bool> _addAimSettings;

        QString _currentStyle;
        QString _currentTheme;
        QMap<QString, QString> _colors;

        quint8 _orderType;
    };
}


#endif // USERSETTINGS_H
