#include "dragmodel.h"
#include <QMimeData>
#include <qdebug.h>

#include "commondefine.h"

DragModel::DragModel()
{

}

DragModel::~DragModel()
{

}

Qt::ItemFlags DragModel::flags(const QModelIndex& index)
{
   Qt::ItemFlags flags = QAbstractItemModel::flags(index);
   flags = flags | Qt::ItemIsDragEnabled;
   qDebug()<<"return flags is "<<flags;
   return flags;
}

QMimeData* DragModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.count() <= 0)
        return 0;

    // 选出各行id
    QSet<QString> idSet;
    for (int i = 0; i < indexes.count(); ++i)
    {
        const QStandardItem* item = this->item(indexes.at(i).row());
        idSet.insert(item->text());
    }

    // 拼成字符串
    QString ids;
    for (QSet<QString>::iterator iter = idSet.begin();iter != idSet.end();++iter)
    {
        ids.append(*iter);
        ids.append("_");
    }
    // 去掉最后一个"_"
    ids = QString(ids.toStdString().substr(0,ids.size()-1).c_str());

    QMimeData* data = new QMimeData;
    data->setText(ids);
    return data;
}

bool DragModel::isDropable(QModelIndex index)
{
    if (this->item(index.row(),index.column())->background()==QColor(250,255,240))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 更新model里面显示的辐照点顺序
void DragModel::exchangeRows(const QMap<QString,QString>& vIds,int to)
{
    QString insertId = this->item(to,0)->text();
    QList<QString> temp;
    QMap<QString,QVector<QString> > itemBak;
    QMap<QString,QBrush> brushBak;
    // 保存当前参数
    for (int i = 0; i < this->rowCount(); ++i)
    {
        temp.append(this->item(i,0)->text());
        QVector<QString> vBak;
        for (int j = 0; j < 5; ++j)
        {
            vBak.push_back(this->item(i,j)->text());
        }

        // 备份参数
        itemBak.insert(this->item(i,0)->text(),vBak);
        // 备份背景颜色
        brushBak.insert(this->item(i,0)->text(),this->item(i,0)->background());
    }

    // 调整顺序
    for (QList<QString>::iterator listIter = temp.begin(); listIter!= temp.end(); ++listIter)
    {
        for (QMap<QString,QString>::const_iterator mapIter = vIds.begin(); mapIter != vIds.end(); ++mapIter)
        {
            if (mapIter.key()==*listIter)
            {
                temp.erase(listIter);
                listIter = temp.begin();
                break;
            }
        }
    }

    // 插入需要调整的ID
    for (QList<QString>::iterator listIter = temp.begin(); listIter!= temp.end(); ++listIter)
    {
        if (*listIter==insertId)
        {
            QList<QString>::iterator pos = listIter;
            for (QMap<QString,QString>::const_iterator mapIter = vIds.begin(); mapIter != vIds.end(); ++mapIter)
            {
                listIter = temp.insert(pos,mapIter.key());
            }

            break;
        }
    }

    // 调整刷新后的model
    for (int i = 0; i < temp.size(); ++i)
    {
        QVector<QString>& vItems = *itemBak.find(temp[i]);
        QBrush& brush = *brushBak.find(temp[i]);

        if (vItems.empty())
        {
           continue;
        }

        for (int j = 0; j < 3; ++j)
        {
            this->item(i,j)->setText(vItems[j]);
            this->item(i,j)->setBackground(brush);
            this->item(i,j)->setEditable(false);
        }

        for (int j = 3; j < 5; ++j)
        {
            this->item(i,j)->setText(vItems[j]);
            this->item(i,j)->setBackground(brush);
        }

        this->setItem(i,5,new QStandardItem(QString("")));
        if (brush!=COMPLETE_COLOR)
        {
            this->item(i,5)->setBackground(QColor(0,255,0));
        }
        else
        {
            this->item(i,5)->setBackground(brush);
        }
    }

    // model刷新后的id排列顺序
    QVector<QString> newIds;
    for (QList<QString>::iterator listIter = temp.begin(); listIter!= temp.end(); ++listIter)
    {
        newIds.push_back(*listIter);
    }

    emit refreshDisplayPlan(newIds);
}
