#ifndef USERTOKEN_H
#define USERTOKEN_H

#include <QObject>

namespace aim {

    class UserToken : public QObject
    {
        Q_OBJECT

    public:
        explicit UserToken(QObject *parent = nullptr);

        Q_INVOKABLE QString getToken() const { return _token; }

    private:
        void prepareToken();

        QString _token;
    };

}

#endif // USERTOKEN_H
