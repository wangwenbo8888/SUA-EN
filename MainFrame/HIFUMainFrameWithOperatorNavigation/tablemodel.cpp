#include "tablemodel.h"
#include <QSqlRecord>
#include <QDebug>

//#include "log.h"
//#include "constant.h"
//#include "mainwindow.h"

TableModel::TableModel(QObject *parent) : QSqlTableModel(parent)
{
}

QVariant TableModel::data ( const QModelIndex & index, int role ) const
{

    if(index.row() >= rowCount())
    {
        return QString("");
    }
    if(role < Qt::UserRole)
    {
        return QSqlTableModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        return QSqlTableModel::data(modelIndex, Qt::DisplayRole);
    }
}

void TableModel::generateRoleNames()
{
    m_roleNames.clear();
    qDebug()<<"column count is "<< columnCount();
    for (int i = 0; i < columnCount(); i++)
    {
        qDebug()<<"role name is " << headerData(i, Qt::Horizontal).toByteArray();
        //qCDebug(MAINWINDOW())<<"role name is " << headerData(i, Qt::Horizontal).toByteArray() << TRACE_CMH();
        m_roleNames.insert(Qt::UserRole + i + 1, headerData(i, Qt::Horizontal).toByteArray());
//        roles[Qt::UserRole + i + 1] = headerData(i, Qt::Horizontal).toString();
    }
}
