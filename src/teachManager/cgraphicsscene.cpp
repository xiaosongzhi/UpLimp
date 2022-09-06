#include "cgraphicsscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
CGraphicsScene::CGraphicsScene():isEdit(false)
{

}
CGraphicsScene::CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent)
    :isEdit(false)
{
    Q_UNUSED(parent)
    setSceneRect(x,y,width,height);
}

void CGraphicsScene::setEditState(bool ok)
{
    isEdit = ok;
}

#if 1
void CGraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
{
    Q_UNUSED(keyEvent)
    qDebug()<<"keyPressEvent";
}

void CGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(isEdit)
    {
        isEdit = false;
        emit signalFinishedEdit();
    }
    return QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(isEdit)
    {
        emit signalSendDestPos(mouseEvent->scenePos());
    }
    return QGraphicsScene::mousePressEvent(mouseEvent);
}
#endif
