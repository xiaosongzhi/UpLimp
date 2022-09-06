#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QObject>

class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CGraphicsScene();
    CGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr);

    void setEditState(bool ok);


protected:
#if 1
    virtual void keyPressEvent(QKeyEvent *keyEvent);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
#endif
private:
    bool isEdit;

signals:
    void signalSendDestPos(QPointF scenePos);
    void signalFinishedEdit();
};

#endif // CGRAPHICSSCENE_H
