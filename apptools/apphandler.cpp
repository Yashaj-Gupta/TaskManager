#include "apphandler.h"

#include <QDebug>
#include <QMessageBox>

#include "app/libinit.h"
#include <windows.h>

void AppHandler::defaultHandle(int argc, char *argv[])
{

      if (argc > 1)
      {
          QString argument = argv[1];

          if (argument == "-noconsole")
          {
              //turn off cout into some file or like that
              ///better in the end make possible to turn console as fist arg,
              /// and second one is command
              fclose(stdout);
          }

          if (argument == "-noupdate")
          {

          }

          if (argument == "-updated")
          {
              QString message = "Новая версия установлена: " + getApplicationVersion()+ "\n";

              QMessageBox msgBox;
              msgBox.setText(message);
              msgBox.exec();
          }

          if (argument == "-version")
          {
              qDebug() << getApplicationVersion(); //version number
              appNeedQuit = true;
          }

      }

      //after that we can switch off console if we don't need it
        #ifdef QT_DEBUG
        ;
        #else
        FreeConsole(); //on release it appears, but diapears on debug
        #endif
}
