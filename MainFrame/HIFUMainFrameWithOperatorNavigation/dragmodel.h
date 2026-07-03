#ifndef DRAGMODEL_H_
#define DRAGMODEL_H_

#include <QStandardItemModel>

#include <QtGui>
#include <QDrag>

class DragModel : public QStandardItemModel
{
    Q_OBJECT

public:

    DragModel();
    ~DragModel();

    Qt::ItemFlags flags(const QModelIndex& index);

    QMimeData* mimeData(const QModelIndexList& indexes) const;

    bool isDropable(QModelIndex index);

public slots:

    void exchangeRows(const QMap<QString,QString>& vIds,int row);

signals:
    void refreshDisplayPlan(const QVector<QString>& ids);

};

#endif // DRAGMODEL

