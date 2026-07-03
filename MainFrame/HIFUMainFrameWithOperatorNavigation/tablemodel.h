#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QHash>
#include <QSqlDatabase>
#include <QSqlTableModel>

class TableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = 0);
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    void generateRoleNames();
    QHash<int, QByteArray> roleNames() const {return m_roleNames;}

private:
    QHash<int, QByteArray> m_roleNames;
};

#endif // TABLEMODEL_H
