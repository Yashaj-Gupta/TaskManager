#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QAbstractItemModel>
#include <QModelIndex>

namespace apptools {


    class TreeItem
    {

    public:
        explicit TreeItem(const QVector<QVariant> &data, int columnsCount, TreeItem *parentItem = nullptr);
        ~TreeItem();

        void appendChild(TreeItem *child);

        TreeItem *child(const int row);
        int childCount() const;

        int columnCount() const;
        QVariant data(int column) const;

        int childNumber() const;
        TreeItem *parent();

        bool setData(const int column, const QVariant &value);
        bool removeChildren(const int position, const int count);

        QString exportColumns() const;
        QStringList getColumns() const;

        bool setValueOnCondition(const int conditionIndex, const QString conditionText,
                                 const int settingIndex, const QString settingText);

        bool setValueOnConditionIfEmpty(const int conditionIndex, const QString conditionText,
                                        const int settingIndex, const QString settingText);

    private:
        QList<TreeItem*> _childItems;
        QVector<QVariant> _itemData;
        TreeItem *_parentItem;
    };



    class TreeModel : public QAbstractItemModel
    {
        Q_OBJECT

    public:

        explicit TreeModel(QObject *parent = nullptr, QStringList columnsList = QStringList());
        ~TreeModel();

        QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

        int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

        int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
        QHash<int, QByteArray> roleNames() const override;


        bool setData(const QModelIndex &index, const QVariant &value,
                     const int role = Qt::EditRole) Q_DECL_OVERRIDE;

        bool setHeaderData(const int section, const Qt::Orientation orientation,
                           const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

        bool removeRows(const int position, const int rows,
                        const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;


        Q_INVOKABLE QString getFieldByIndex(const QModelIndex index, const int field) const;

        Q_INVOKABLE bool printTopValues() const;

        Q_INVOKABLE QString getAimId(const QModelIndex &index);

        Q_INVOKABLE bool addTopItem(const QModelIndex &index);
        Q_INVOKABLE bool addChildItem(const QModelIndex &index);
        Q_INVOKABLE bool setItemText(const QModelIndex &index, const QString text, int column=0);
        Q_INVOKABLE bool removeItem(const QModelIndex &index);

        Q_INVOKABLE QString getColumnName(const int index);
        Q_INVOKABLE int getColumnIndex(const QString name);
        Q_INVOKABLE int columnsAmount();


        Q_INVOKABLE QString getModelDataString() const;
        void setModelDataString(const QString &linesData);


        Q_INVOKABLE void fillWithAimList(QVariantList allAims);
        Q_INVOKABLE void fillWithTagList(QVariantList allTags);


        Q_INVOKABLE void saveToFile(const QString filename="") const;
        Q_INVOKABLE void loadFromFile(const QString filename="");

        QVariantList getFullList() const;


        bool setValueOnCondition(const int conditionIndex, const QString conditionText,
                                 const int settingIndex, const QString settingText);

        bool setValueOnConditionIfEmpty(const int conditionIndex, const QString conditionText,
                                        const int settingIndex, const QString settingText);

        TreeItem* getFirstItem() const {
            if (_rootItem->childCount())
                return _rootItem->child(0);
            return 0;
        }

       void refillTableColumns(const QStringList data);
       void setNewEntryTemplate(const QString newTempate) { newEntryTemplate = newTempate; }

       TreeItem *getRootItem() const { return _rootItem; }
       size_t getColumnNamesSize() const { return _columnNames.size(); }

    private:

       QString newEntryTemplate;

        bool setDataSpecial(const QModelIndex& index, const QVariant &value, const int column);
        TreeItem *getItem(const QModelIndex& index) const;

        void setupModelData(const QString& linesData, TreeItem *parent);
        void addChildrenExport(QStringList& outputList, TreeItem* parent, const int level) const;

        void addChildrenList(QVariantList& outputList, TreeItem* parent) const;

        TreeItem* _rootItem;
        QHash<int, QByteArray> _roleNameMapping;

    protected:
        void fillTableColumns();
        void autoFillRoles();
        void createRootElement();

        QList<QString> _columnNames;
    };
}


#endif // TREEMODEL_H
