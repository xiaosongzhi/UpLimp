#include "cbasewidget.h"
#include "ui_cbasewidget.h"
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>

CBaseWidget::CBaseWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CBaseWidget)
    , m_isMousePressed(false)
    , E_cursePosition(LEFTTOP)
    , m_lastGlobalPoint(0,0)
    , m_eventPoint(0,0)

{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setWindowOpacity(0.99);
    installEventFilter();

    m_originalRect = rect();

    connect(ui->title_Widget,&TitleForm::signalWindowMove,this,&CBaseWidget::slotWindowMove);
    connect(ui->title_Widget,&TitleForm::signalSwitchWindowSize,this,&CBaseWidget::slotSwitchWindowSize);
    connect(ui->title_Widget,&TitleForm::signalButtonEvent,this,&CBaseWidget::slotButtonEvent);
}

CBaseWidget::~CBaseWidget()
{
    delete ui;
}


void CBaseWidget::on_close_Btn_clicked()
{
    this->close();
}

void CBaseWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
}

void CBaseWidget::mouseMoveEvent(QMouseEvent *event)
{

    //当鼠标按下并进行拖拽时
    if(m_isMousePressed)
    {
        switch(E_cursePosition)
        {
        case LEFTTOP:
        {
            int xRang = event->globalPos().x() - m_lastGlobalPoint.x();
            int yRang = event->globalPos().y() - m_lastGlobalPoint.y();
            setGeometry(event->globalPos().x(),event->globalPos().y(),m_originalRect.width()-xRang,m_originalRect.height()-yRang);
        }
            break;
        case TOP:
        {
            int xRang = m_lastGlobalPoint.x() - m_eventPoint.x();
            int yRang = event->globalPos().y() - m_lastGlobalPoint.y();
            setGeometry(m_lastGlobalPoint.x()-xRang,event->globalPos().y(),m_originalRect.width(),m_originalRect.height()-yRang);
        }
            break;
        case RIGHTTOP:
        {
            int xRang = event->globalPos().x() - m_lastGlobalPoint.x();
            int yRang = event->globalPos().y() - m_lastGlobalPoint.y();
            setGeometry(m_lastGlobalPoint.x()-m_originalRect.width(),event->globalPos().y(),m_originalRect.width()+xRang,m_originalRect.height()-yRang);
        }
            break;
        case LEFT:
        {
            int xRang = event->globalPos().x() - m_lastGlobalPoint.x();
            setGeometry(event->globalPos().x(),m_lastGlobalPoint.y()-m_eventPoint.y(),m_originalRect.width()-xRang,m_originalRect.height());
        }
            break;
        case RIGHT:
        {
            int xRang = event->globalPos().x() - m_lastGlobalPoint.x();
            setGeometry(m_lastGlobalPoint.x()-m_originalRect.width(),m_lastGlobalPoint.y()-m_eventPoint.y(),m_originalRect.width()+xRang,m_originalRect.height());
        }
            break;
        case LEFTBOTTOM:
        {
            int xRang = event->globalPos().x() - m_lastGlobalPoint.x();
            int yRang = event->globalPos().y() - m_lastGlobalPoint.y();
            setGeometry(event->globalPos().x(),m_lastGlobalPoint.y()-m_eventPoint.y(),m_originalRect.width()-xRang,m_originalRect.height()+yRang);
        }
            break;
        case BOTTOM:
        {
            int yRang = event->globalPos().y() - m_lastGlobalPoint.y();
            setGeometry(m_lastGlobalPoint.x()-m_eventPoint.x(),m_lastGlobalPoint.y()-m_eventPoint.y(),m_originalRect.width(),m_originalRect.height()+yRang);
        }
            break;
        case RIGHTBOTTOM:
        {
            int xRang = event->globalPos().x() - m_lastGlobalPoint.x();
            int yRang = event->globalPos().y() - m_lastGlobalPoint.y();
            setGeometry(m_lastGlobalPoint.x()-m_eventPoint.x(),m_lastGlobalPoint.y()-m_eventPoint.y(),m_originalRect.width()+xRang,m_originalRect.height()+yRang);
        }
            break;
        }
    }

}
bool CBaseWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::MouseButtonPress)
    {
        m_isMousePressed = true;
    }
    else if(ev->type() == QEvent::MouseButtonRelease)
    {
        m_isMousePressed = false;
    }
    else if(ev->type() == QEvent::Enter)
    {
        setMouseCurse(obj);
    }
    else if(ev->type() == QEvent::Leave)
    {
        setCursor(Qt::ArrowCursor);
    }
    return QWidget::eventFilter(obj, ev);
}

void CBaseWidget::setMouseCurse(QObject *obj)
{
    if(obj == ui->leftTop_Label) //左上角
    {
        E_cursePosition = LEFTTOP;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(obj == ui->rightBottom_Label)//右下角
    {
        E_cursePosition = RIGHTBOTTOM;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(obj == ui->rightTop_Label) //右上角
    {
        E_cursePosition = RIGHTTOP;
        setCursor(Qt::SizeBDiagCursor);
    }
    if(obj == ui->leftBottom_Label) //左下角
    {
        E_cursePosition = LEFTBOTTOM;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if(obj == ui->left_Label)//左
    {
        E_cursePosition = LEFT;
        setCursor(Qt::SizeHorCursor);
    }
    else if(obj == ui->right_Label)//右
    {
        E_cursePosition = RIGHT;
        setCursor(Qt::SizeHorCursor);
    }
    else if(obj == ui->bottom_Label)//下
    {
        E_cursePosition = BOTTOM;
        setCursor(Qt::SizeVerCursor);
    }
    else if(obj == ui->top_Label) //上
    {
        E_cursePosition = TOP;
        setCursor(Qt::SizeVerCursor);
    }
}

void CBaseWidget::installEventFilter()
{
    ui->leftTop_Label->installEventFilter(this);
    ui->top_Label->installEventFilter(this);
    ui->rightTop_Label->installEventFilter(this);
    ui->left_Label->installEventFilter(this);
    ui->right_Label->installEventFilter(this);
    ui->leftBottom_Label->installEventFilter(this);
    ui->bottom_Label->installEventFilter(this);
    ui->rightBottom_Label->installEventFilter(this);
}

void CBaseWidget::slotWindowMove(QPoint point)
{
    if(this->isMaximized())
        return;
    this->move(point);

}

void CBaseWidget::slotSwitchWindowSize()
{
    if(this->isMaximized())
    {
        this->showNormal();
    }
    else
        this->showMaximized();
}

void CBaseWidget::slotButtonEvent(E_BUTTONEVENT event)
{
    switch(event)
    {
    case E_MINEVENT:
        this->showMinimized();
        break;
    case E_MAXEVENT:
        this->showMaximized();
        break;
    case E_HELPEVENT:
        //自定义
        break;
    case E_QUITEVENT:
        this->close();
        break;
    }
}

void CBaseWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastGlobalPoint = event->globalPos();
    m_originalRect = rect();
    m_eventPoint = event->pos();
}
