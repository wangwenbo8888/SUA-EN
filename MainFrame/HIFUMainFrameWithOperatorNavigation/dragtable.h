#ifndef DRAGTABLE_H_
#define DRAGTABLE_H_

#include <QTableView>
#include <QStandardItemModel>
#include <QTableWidget>

#include <QtGui>
#include <QDrag>

class DragTable : public QTableView
{
    Q_OBJECT

public:
    DragTable(QWidget* parent = NULL);
    ~DragTable();

protected:

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);

    void dragMoveEvent(QDragMoveEvent *event);

    void dropEvent(QDropEvent *event);

signals:

    void exchangeRows(const QMap<QString,QString>& from,int row);

private:

    QStandardItemModel *m_pModel;
    QByteArray data;

    QPoint startPos;
};

#endif // DRAGTABLE_H_

