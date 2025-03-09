#include "TreeModel.h"

#include <QStringList>
#include <QDebug>
#include <QFile>


using namespace apptools;

Q_DECLARE_METATYPE(QModelIndex)

TreeItem::TreeItem(const QVector<QVariant> &data, int columnsCount, TreeItem *parent) {
    _parentItem = parent;
    _itemData = data;

    if (_itemData.size() < columnsCount)
        for (int i = _itemData.size()-1; i < columnsCount; ++i)
            _itemData.append(" ");
}


TreeItem::~TreeItem() {
    qDeleteAll(_childItems);
}


void TreeItem::appendChild(TreeItem *item) {
    _childItems.append(item);
}


TreeItem *TreeItem::child(const int row) {
    return _childItems.value(row);
}


int TreeItem::childCount() const {
    return _childItems.count();
}


int TreeItem::columnCount() const {
    return _itemData.count();
}


QVariant TreeItem::data(int column) const {
    return _itemData.value(column);
}


TreeItem *TreeItem::parent() {
    return _parentItem;
}


int TreeItem::childNumber() const {
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}


QString TreeItem::exportColumns() const {
    QString result;
    for (int i = 0; i < columnCount(); ++i) {
        result += data(i).toString();
        QString sepparator(4,'\t');
        if (i!= columnCount()-1)
            result += sepparator;
    }
    return result;
}


QStringList TreeItem::getColumns() const {
    QStringList response;
    for (int i = 0; i < columnCount(); ++i)
        response << data(i).toString();
    return response;
}


bool TreeItem::removeChildren(int position, const int count) {
    if (position < 0 || position + count > _childItems.size())
        return false;
    for (int row = 0; row < count; ++row)
        delete _childItems.takeAt(position);
    return true;
}


bool TreeItem::setData(const int column, const QVariant &value) {
    if (column < 0)
        return false;

    QString insertingValue = value.toString();
    insertingValue = insertingValue.trimmed();
    if (insertingValue.isEmpty())
        insertingValue = " ";

    if (column >= _itemData.size()) {
        _itemData.append(insertingValue);
        return true;
    }

    _itemData[column] = insertingValue;
    return true;
}


TreeModel::TreeModel(QObject *parent, QStringList columnsList)
    : QAbstractItemModel(parent),_rootItem(0)
{
    newEntryTemplate = " ";

    if (columnsList.size()==0)
    {
        fillTableColumns();
        autoFillRoles();
        createRootElement();
    }
    else
        refillTableColumns(columnsList);
}


void TreeModel::refillTableColumns(const QStringList data) {
    _columnNames = data;
    autoFillRoles();
    createRootElement();
}


void TreeModel::fillTableColumns()
{
    _columnNames.clear();

    _columnNames.append("Aim name");
    _columnNames.append("Time");
    _columnNames.append("Date");
    _columnNames.append("Comment");

    //columnNames.append("flags");
}

void TreeModel::autoFillRoles()
{
    _roleNameMapping.clear();

    for (int i = 0; i < _columnNames.size(); ++i)
         _roleNameMapping[Qt::UserRole + 1 + i]= _columnNames[i].toUtf8();
}

void TreeModel::createRootElement()
{
    QVector<QVariant> rootData;
    for (int i = 0; i < _columnNames.size(); ++i)
        rootData << _columnNames[i];

    if (_rootItem)
    {
        delete _rootItem;
        _rootItem = 0;
    }

    _rootItem = new TreeItem(rootData, _columnNames.size()); //Qt is far from modern C++.. at least Qt5
}


QString TreeModel::getColumnName(const int index)
{
    if (index < 0 || index >= _columnNames.size())
        return "";
    return _columnNames[index];
}


int TreeModel::getColumnIndex(const QString name) {
    for (int i = 0; i < _columnNames.size(); ++i)
        if (_columnNames[i] == name)
            return i;
    return -1;
}


int TreeModel::columnsAmount()
{
    return _columnNames.size() ;
}

TreeModel::~TreeModel()
{
    if (_rootItem)
    {
        delete _rootItem;
        _rootItem = 0;
    }
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return _rootItem->columnCount();
}


QString TreeModel::getFieldByIndex(const QModelIndex index, const int field) const
{
    TreeItem *item = getItem(index);
    if (item->columnCount() > field)
        return item->data(field).toString();

    return "";
}


QVariant TreeModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(role - Qt::UserRole - 1);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    return _roleNameMapping;
}

void TreeModel::setupModelData(const QString &linesData, TreeItem *parent)
{
    QStringList  lines = linesData.split(QString("\n"));

    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {

        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {

            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            parents.last()->appendChild(new TreeItem(columnData, _columnNames.size(), parents.last()));
        }

        ++number;
    }
}

//=========import + export============

QString TreeModel::getModelDataString() const
{
    QStringList exportData;

    for (int i = 0; i < _rootItem->childCount(); ++i)
    {
        TreeItem *currentItem = _rootItem->child(i);
        QString oneLine = currentItem->exportColumns();

        exportData.append(oneLine);

        if (currentItem->childCount())
            addChildrenExport(exportData,currentItem,1);
    }

    QString response;

    for (int i = 0; i < exportData.size(); ++i)
        response += exportData[i] + "\n";

    return response;
}


QStringList putIdOnFirstPlace(QStringList oldList)
{
    QStringList lineWithFineId;

    if (oldList.size())
    {
        lineWithFineId << oldList[oldList.size()-1];

        for (int j = 0; j < oldList.size()-1; ++j)
            lineWithFineId << oldList[j];
    }
    return lineWithFineId;
}

QVariantList TreeModel::getFullList() const
{
    QVariantList response;
    for (int i = 0; i < _rootItem->childCount(); ++i) {
        TreeItem *currentItem = _rootItem->child(i);
        QStringList oneLine = currentItem->getColumns();

        QStringList lineWithFineId;
        lineWithFineId = putIdOnFirstPlace(oneLine);
        response << lineWithFineId;

        if (currentItem->childCount())
            addChildrenList(response,currentItem);
    }

    return response;
}

bool TreeModel::setValueOnCondition(int conditionIndex, const QString conditionText,
                         const int settingIndex, const QString settingText)
{
    //MOST SIMPLE WAY EVER
    return _rootItem->setValueOnCondition(conditionIndex,conditionText,settingIndex,settingText);
}

bool TreeItem::setValueOnCondition(const int conditionIndex, const QString conditionText, const int settingIndex, const QString settingText)
{
    if (_itemData.size() > conditionIndex)
    {
        if (_itemData[conditionIndex].toString() == conditionText)
        {
            if (_itemData.size() > settingIndex)
            {
                _itemData[settingIndex] = settingText;
                return true;
            }
            else
                return false;
        }
    }

    for (int i = 0; i < _childItems.size(); ++i)
    {
        if (_childItems[i]->setValueOnCondition(conditionIndex,conditionText,settingIndex, settingText)) //recurse, but it wouldn't grow too mush
            return true;
    }

    return false;
}


bool TreeModel::setValueOnConditionIfEmpty(const int conditionIndex, const QString conditionText,
                         const int settingIndex, const QString settingText)
{
    return _rootItem->setValueOnConditionIfEmpty(conditionIndex,conditionText,settingIndex,settingText);
}


bool TreeItem::setValueOnConditionIfEmpty(const int conditionIndex, const QString conditionText,
                                          const int settingIndex, const QString settingText)
{
    if (_itemData.size() > conditionIndex)
        if (_itemData[conditionIndex].toString() == conditionText)
        {
            if (_itemData.size() > settingIndex)
            {
                QString tempString = _itemData[settingIndex].toString().trimmed();
                if (tempString.isEmpty())  {
                    _itemData[settingIndex] = settingText;
                    return true;
                }
                else
                    return false;
            }
            else
                return false;
        }

    for (int i = 0; i < _childItems.size(); ++i)
        if (_childItems[i]->setValueOnConditionIfEmpty(conditionIndex,conditionText,settingIndex, settingText)) //recurse, but it wouldn't grow too mush
            return true;
    return false;
}



void TreeModel::addChildrenList(QVariantList &outputList, TreeItem *parent) const
{
    QStringList parentLine = parent->getColumns();
    QString parentCode =  parentLine[1];

    for (int i = 0; i < parent->childCount(); ++i) {
        TreeItem *currentItem = parent->child(i);
        QStringList oneLine = currentItem->getColumns(); //There was parent code, but its unused
        QStringList lineWithFineId;
        lineWithFineId = putIdOnFirstPlace(oneLine);
        outputList.append(lineWithFineId);

        if (currentItem->childCount())
            addChildrenList(outputList,currentItem);
    }
}


void TreeModel::addChildrenExport(QStringList &outputList, TreeItem *parent, const int level) const
{
    for (int i = 0; i < parent->childCount(); ++i) {
        TreeItem *currentItem = parent->child(i);
        QString oneLine = currentItem->exportColumns();

        QString spaces(4*level,' ');

        oneLine = spaces + oneLine;
        outputList.append(oneLine);

        if (currentItem->childCount())
            addChildrenExport(outputList,currentItem,level+1);
    }
}


void TreeModel::setModelDataString(const QString &linesData) {

    beginResetModel();

    if (_rootItem) {
        delete _rootItem;
        _rootItem = 0;
    }

    createRootElement();
    endResetModel();;

    beginResetModel();
    setupModelData(linesData,_rootItem);
    endResetModel();
}


void TreeModel::fillWithAimList(QVariantList allAims) {

    beginResetModel();

    if (_rootItem) {
        delete _rootItem;
        _rootItem = 0;
    }

    createRootElement();
    endResetModel();;
    beginResetModel();


    QVariantList rootAims;
    QList<TreeItem*> treeItems;

    for (int i = 0 ; i < allAims.size(); ++i) {
        QStringList aimList = allAims[i].toStringList();
        QString parentId = aimList[10];

        if (parentId.isEmpty()) {
            rootAims << aimList;
            allAims.removeAt(i);
            --i;

            QVector<QVariant> listPreparedForTree;
            for (int k = 1; k < aimList.size(); ++k) {
                QVariant value = aimList[k];
                listPreparedForTree << value;
            }
            listPreparedForTree << aimList[0];

            TreeItem *item = new TreeItem(listPreparedForTree, getColumnNamesSize(), getRootItem());
            treeItems << item;
            getRootItem()->appendChild(item);
        }
    }

    while (rootAims.empty()==false && allAims.size() > 0)
    {
        QVariantList nextRootAims;
        QList<TreeItem*> nextTreeItems;

        for (int i = 0; i < rootAims.size(); ++i)
        {
            QStringList aimList = rootAims[i].toStringList();
            QString aimId = aimList[0];

            for (int j = 0; j < allAims.size(); ++j)
            {
                QStringList nextAimList = allAims[j].toStringList();
                QString parentId = nextAimList[10];

                if (parentId == aimId)
                {
                    nextRootAims << nextAimList;
                    allAims.removeAt(j);
                    --j;

                    TreeItem *parentItem = treeItems[i];
                    QVector<QVariant> listPreparedForTree;

                    for (int k = 1; k < nextAimList.size(); ++k) {
                        QVariant value = nextAimList[k];
                        listPreparedForTree << value;
                    }
                    listPreparedForTree << nextAimList[0]; //put AimId int the end

                    TreeItem *item = new TreeItem(listPreparedForTree,getColumnNamesSize(),parentItem); ///CONSTRUCTOR!
                    nextTreeItems << item;
                    parentItem->appendChild(item);
                }
            }
        }
        rootAims = nextRootAims;
        treeItems = nextTreeItems;
    }

    endResetModel();
}


void TreeModel::fillWithTagList(QVariantList allTags)
{

    beginResetModel();

    if (_rootItem) {
        delete _rootItem;
        _rootItem = 0;
    }

    createRootElement();
    endResetModel();;

    beginResetModel();

    QVariantList rootTags;
    QList<TreeItem*> treeItems;

    for (int i = 0 ; i < allTags.size(); ++i) {
        QStringList tagList = allTags[i].toStringList();
        QString parent = tagList[1];

        if (parent.isEmpty()) {
            rootTags << tagList;
            allTags.removeAt(i);
            --i;

            QVector<QVariant> listPreparedForTree;
            for (int k = 0; k < tagList.size(); ++k) {
                QVariant value = tagList[k];
                listPreparedForTree << value;
            }

            TreeItem *item = new TreeItem(listPreparedForTree, getColumnNamesSize(), getRootItem());
            treeItems << item;
            getRootItem()->appendChild(item);

        }
    }

    while (rootTags.empty()==false && allTags.size() > 0) {

        QVariantList nextRootTags;
        QList<TreeItem*> nextTreeItems;

        for (int i = 0; i < rootTags.size(); ++i) {
            QStringList tagList = rootTags[i].toStringList();
            QString tagName = tagList[0];

            for (int j = 0; j < allTags.size(); ++j) {
                QStringList nextTagList = allTags[j].toStringList();
                QString parent = nextTagList[1];

                if (parent == tagName) {
                    nextRootTags << nextTagList;
                    allTags.removeAt(j);
                    --j;

                    TreeItem *parentItem = treeItems[i];

                    QVector<QVariant> listPreparedForTree;

                    for (int k = 0; k < nextTagList.size(); ++k) {
                        QVariant value = nextTagList[k];
                        listPreparedForTree << value;
                    }

                    TreeItem *item = new TreeItem(listPreparedForTree,getColumnNamesSize(),parentItem); ///CONSTRUCTOR!
                    nextTreeItems << item;
                    parentItem->appendChild(item);
                }
            }
        }

        rootTags = nextRootTags;
        treeItems = nextTreeItems;
    }

    endResetModel();
}


void TreeModel::saveToFile(const QString filename) const
{
    if (filename.isEmpty())
        return;

    QFile outputFile;
    outputFile.setFileName(filename);
    outputFile.open(QIODevice::WriteOnly);
    QString exportData = getModelDataString();
    outputFile.write(exportData.toUtf8());
    outputFile.close();
}

void TreeModel::loadFromFile(const QString filename)
{
    if (filename.isEmpty())
        return;

    QFile inputFile;
    inputFile.setFileName(filename);
    inputFile.open(QIODevice::ReadOnly);
    QString importData = inputFile.readAll();
    setModelDataString(importData);
    inputFile.close();
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, const int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}


bool TreeModel::setDataSpecial(const QModelIndex &index, const QVariant &value, const int column)
{
    TreeItem *item = getItem(index);
    bool result = item->setData(column, value);

    if (result)
        emit dataChanged(index, index);

    return result;
}


bool TreeModel::setHeaderData(const int section, const Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = _rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}


bool TreeModel::removeRows(const int position, const int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return _rootItem;
}



bool TreeModel::printTopValues() const {
    for (int i = 0; i < _rootItem->childCount(); ++i) {
        TreeItem *currentItem = _rootItem->child(i);
        QString fullLine;
        for (int j = 0; j < currentItem->columnCount(); ++j)
            fullLine += " [" + QString::number(j) + "] " + currentItem->data(j).toString();
        qDebug() << fullLine;
    }
    return true;
}

bool TreeModel::addTopItem(const QModelIndex &index)
{
    Q_UNUSED(index);

    QVector<QVariant> handData;
    handData << newEntryTemplate << " " << " "; //position?

    TreeItem *item = _rootItem;
    TreeItem *handMade = new TreeItem(handData, _columnNames.size(),item);

    beginInsertRows(QModelIndex(),item->childCount(),item->childCount());
    item->appendChild(handMade);
    endInsertRows();

    return true;
}

bool TreeModel::addChildItem(const QModelIndex &index)
{
    QVector<QVariant> handData;
    handData << newEntryTemplate << " " << " "; //position?

    TreeItem *item = getItem(index);
    TreeItem *handMade = new TreeItem(handData, _columnNames.size(), item);

    beginInsertRows(index,item->childCount(),item->childCount());
    item->appendChild(handMade);
    endInsertRows();

    return true;
}

QString TreeModel::getAimId(const QModelIndex &index)
{
    TreeItem *item = getItem(index);
    return item->data(12).toString(); //PLEASE INSURE THE INDEX and ENUMERATE
}

bool TreeModel::setItemText(const QModelIndex &index, const QString text, int column)
{
    setDataSpecial(index,text,column);

    return true;
}

bool TreeModel::removeItem(const QModelIndex &index)
{
    if (index == QModelIndex())
        return false;

    removeRow(index.row(),index.parent());
    return true;
}
