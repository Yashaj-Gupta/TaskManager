#include "statemachine.h"

#include <QList>

void insert(int id, QString name, QList<int> items)
{

}

StateMachine::StateMachine()
{
    //lets imagine universal way to be able to use it in many applications later
    //most base model:
    ///SO after all most simple way is explicit - just set some id, name, and set of connections

    //insertKnot(1,"initial state",2); //single out could be used as int
    //insertKnot(2,"init libs",3);
    //insertKnot(3,"main cycle","3,4"); //can make also QList<int> and split it from QString - to QStringList by "," and trimming
    //insertKnot(4,"command interuption",5);
    //insertKnot(5,"command handle",3);

    //but in fact state 5 can make many different state, or better option - to affect sub-StateMachine
    //for example on birthdays app - top chief or normal user;

}


//So this is simple shape of state machine, but it has to be related at leas to few other places, lets take a look:

void runStep()
{
    //SO if command have single output - then everything is pretty simple, all thats had to be done - call function
    //but if there are options on output knots so it has to be return value which to go next, or have some special condition function

    /// for example on main cycle - there has to be check does we recieve any command interuption which could be one of:
    //1) keyboard press
    //2) mouse single press
    //3) function button
    //4) function button with params


    //SO on step #5 we have to transmit prepared message to Core;

    //There could be few ways of handling message; one that is simple to extend is Strings:
        // then better to make some format
            //  like prefix : values ; sepparated with some sign (;)
                // k: (keyboard) key1;key2;key3; , m(mouse):X,Y,button type; , b: button,


   ///SO before level of interuption something had to give command from outside, used to be from qml
   /// something just like : pushKeyboardCommand(); pushMouseCommand(); pushButton or something like that

  // so it packes command into the string and possible to take *getCommand;
    //so the command and things are inside UserInterfaceInput
    //and graphical UI is UserInterfaceOutput (also sound too)


    //SOOOO should take a depper look on the StateMachine, is there need for suchThings as UserInput & CoreProcess
    //maybe them both could lay there


    ///AND also should thing about testing, because tester is a way of walking all the possible states of state machine,
    /// sending all the possible codes to CoreProcess - imulating user input
}


