#include "unsavedcheck.h"

#include <QMessageBox>


bool UnsavedCheck::forceSaved()
{
    if (saved == false)
    {
       QMessageBox msgBox;
       msgBox.setText("Вы хотите оставить введенные данные без сохранения?");
       msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
       int result = msgBox.exec();

       if (result == QMessageBox::Yes)
           saved = true; //clean up before next time

       return result == QMessageBox::Yes;
    }

    return true;
}
