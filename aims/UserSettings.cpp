#include "UserSettings.h"

using namespace aim;


UserSettings::UserSettings(QObject *parent): QObject(parent)
{
    _orderType = 0;
    setColor("Accent","#4CAF50"); //Material.Green
    setColor("Primary","#3F51B5"); //Material.Ingido
    setColor("Foreground","#444444"); // #795548or Material.Brown
    setColor("Background","#333333"); //or Material.Grey#607D8B
    setColor("ListHightlight","#263E26"); //#98FB98
    setColor("Text","#CCCCCC");
    _addAimSettings["ListName"] = true;
    _addAimSettings["TimeAndDate"] = true;
    _addAimSettings["Category"] = true;
    _addAimSettings["Repeatable"] = true;
    _addAimSettings["Privacy"] = true;
    _addAimSettings["AssignTo"] = true;
    _addAimSettings["ParentAim"] = true;
    _addAimSettings["ChildAims"] = true;
    _addAimSettings["Progress"] = true;
    _viewAimSettings["ListName"] = true;
    _viewAimSettings["TimeAndDate"] = true;
    _viewAimSettings["Category"] = true;
    _viewAimSettings["Repeatable"] = true;
    _viewAimSettings["Privacy"] = true;
    _viewAimSettings["AssignTo"] = true;
    _viewAimSettings["ParentAim"] = true;
    _viewAimSettings["ChildAims"] = true;
    _viewAimSettings["Progress"] = true;
}


QList<bool> UserSettings::getAddAimSettings() const {
    QList<bool> response;
    response.append(_viewAimSettings["ListName"]);
    response.append(_viewAimSettings["TimeAndDate"]);
    response.append(_viewAimSettings["Category"]);
    response.append(_viewAimSettings["Repeatable"]);
    response.append(_viewAimSettings["Privacy"]);
    response.append(_viewAimSettings["AssignTo"]);
    response.append(_viewAimSettings["ParentAim"]);
    response.append(_viewAimSettings["ChildAims"]);
    response.append(_viewAimSettings["Progress"]);
    return response;
}

void UserSettings::setAddAimSettings(const QList<bool> newSettings)
{
    _addAimSettings["ListName"] = newSettings[0];
    _addAimSettings["TimeAndDate"] = newSettings[1];
    _addAimSettings["Category"] = newSettings[2];
    _addAimSettings["Repeatable"] = newSettings[3];
    _addAimSettings["Privacy"] = newSettings[4];
    _addAimSettings["AssignTo"] = newSettings[5];
    _addAimSettings["ParentAim"] = newSettings[6];
    _addAimSettings["ChildAims"] = newSettings[7];
    _addAimSettings["Progress"] = newSettings[8];
}


QList<bool> UserSettings::getViewAimSettings() const
{
    QList<bool> response;

    response.append(_viewAimSettings["ListName"]);
    response.append(_viewAimSettings["TimeAndDate"]);
    response.append(_viewAimSettings["Category"]);
    response.append(_viewAimSettings["Repeatable"]);
    response.append(_viewAimSettings["Privacy"]);
    response.append(_viewAimSettings["AssignTo"]);
    response.append(_viewAimSettings["ParentAim"]);
    response.append(_viewAimSettings["ChildAims"]);
    response.append(_viewAimSettings["Progress"]);

    return response;
}

void UserSettings::setViewAimSettings(const QList<bool> newSettings)
{
    _viewAimSettings["ListName"] = newSettings[0];
    _viewAimSettings["TimeAndDate"] = newSettings[1];
    _viewAimSettings["Category"] = newSettings[2];
    _viewAimSettings["Repeatable"] = newSettings[3];
    _viewAimSettings["Privacy"] = newSettings[4];
    _viewAimSettings["AssignTo"] = newSettings[5];
    _viewAimSettings["ParentAim"] = newSettings[6];
    _viewAimSettings["ChildAims"] = newSettings[7];
    _viewAimSettings["Progress"] = newSettings[8];
}


bool UserSettings::setColor(const QString colorType, const QString colorValue)
{
    bool colorExists = _colors.find(colorType) != _colors.end();
    _colors[colorType]=colorValue;
    return colorExists;
}

QString UserSettings::getColor(QString colorType) const
{
    bool colorExists = _colors.find(colorType) != _colors.end();
    if (colorExists)
        return _colors[colorType];
    return "";
}
