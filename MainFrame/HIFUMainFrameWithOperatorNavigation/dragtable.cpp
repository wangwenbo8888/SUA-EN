#include "dragtable.h"
#include <QHeaderView>
#include "dragmodel.h"

DragTable::DragTable(QWidget* parent)
: QTableView(parent)
{
    setAcceptDrops(true);
}

DragTable::~DragTable()
{

}

void DragTable::mousePressEvent(QMouseEvent *event )
{
    if (event->button() == Qt::LeftButton)
    {
        startPos = event->pos();
    }

    QTableView::mousePressEvent(event);
}

void DragTable::mouseMoveEvent(QMouseEvent *event)
{

    if (event->buttons() & Qt::LeftButton)
    {
        // TODO confim the move distance
        if ((event->pos() - startPos).manhattanLength()< this->rowHeight(1))
            return;

        QDrag* drag = new QDrag(this);
//        QMimeData* mimeData = new QMimeData;
//        mimeData->setData("text/plain",data);
        //drag->setMimeData(mimeData);

        Qt::DropAction resultAction = drag->exec(Qt::MoveAction);
        if (resultAction==Qt::MoveAction)
        {
            // 删除数据源
        }
    }

    QTableView::mouseMoveEvent(event);
}

void DragTable::dragEnterEvent(QDragEnterEvent *event)
{
    DragTable *source = (DragTable *)((void*)(event->source()));
    if (source && source == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void DragTable::dragMoveEvent(QDragMoveEvent *event)
{
    // 超过边界的位置不能拖放
    QModelIndex index = indexAt(event->pos());
    if (index.row() < 0)
    {
        event->ignore();
        return;
    }

    // 相同的行不能拖动
    if (indexAt(event->pos()).row()== indexAt(startPos).row())
    {
        event->ignore();
        return;
    }

    // 只能向前拖，不能向后拖
    if(event->pos().y() > startPos.y())
    {
        event->ignore();
        return;
    }

    DragModel* model = (DragModel*)this->model();
    if (!model->isDropable(index))
    {
        event->ignore();
        return;
    }

    if (event->mimeData()->hasFormat("text/plain"))
    {
        event->acceptProposedAction();
        return;
    }

    event->ignore();
    QTableView::dragMoveEvent(event);
}

void DragTable::dropEvent(QDropEvent *event )
{

    DragTable *source = (DragTable *)((void*)(event->source()));
    if (source == this)
    {
        QModelIndex index = this->indexAt(event->pos());
        QString ids = event->mimeData()->text();

        QMap<QString,QString> vIds;
        QStringList idList = ids.split(QRegExp("_"));
        foreach (QString id, idList)
        {
            // qDebug()<<"drop id is "<<id;
            vIds.insert(id,id);
        }

        emit exchangeRows(vIds,index.row());

        return;
    }

    event->ignore();
    QTableView::dropEvent(event);
}
