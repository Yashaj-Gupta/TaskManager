#ifndef STATEMACHINE_H
#define STATEMACHINE_H


#include <QString>
#include <QList>


class StateKnot
{
    //he we also hold connections

public:

    void addConnection(QString knotName);
    void addConnection(int knotIndex);          //Well... its not well because yet its unfinished

    ///need to think about it a little

protected:

    QString knotName;
    QList<QString> outputKnots;

};


class StateMachine
{
public:
    StateMachine();

protected :

        int addKnot(QString knotName);
        int findKnot(QString knotName);

        void clear();

        bool addConnection(QString knotName1, QString knotName2);
        bool addConnection(int knotId1, int knotId2);

        bool removeConnection(QString knotName1, QString knotName2);
        bool removeConnection(int knotId1, int knotId2);


private:

        QList<StateKnot> knots;
};

#endif // STATEMACHINE_H
