#ifndef TEACHEDIT_H
#define TEACHEDIT_H

#include <QWidget>
#include <QMap>
#include "cbasedialog.h"
class CListWidgetItem;
class QListWidgetItem;
namespace Ui {
class TeachEdit;
}

typedef enum
{
    ADDTYPE = 0,
    EDITTYPE
}E_SHOWTYPE;

typedef struct
{
    QWidget *item;
    QPoint point;
}ST_ItemPoint;

class TeachEdit : public CBaseDialog
{
    Q_OBJECT

public:
    explicit TeachEdit(QWidget *parent = nullptr);
    ~TeachEdit();

    void switchType(E_SHOWTYPE);
    //设置处方
    void setRecipe(QString trackName);
signals:
    //数据发生变化--告知处方管理
    void signalUpdateDatabase();

    //关闭窗口信号
    void signalCloseWindow();
protected:
    void closeEvent(QCloseEvent *event);
    virtual void
    showEvent(QShowEvent *event);

public slots:
    //将新添加的点的位置显示在右侧列表中
    void slotAddNewPoint(QString index,QPoint point);
    //拖拽右侧点，左侧列表中位置进行更新
    void slotNodeChanged(QString text,QPoint point);
    //接收添加点 完成按钮
    void slotNodeAddFinished();
private slots:
    void on_add_Btn_clicked();

    void on_clear_Btn_clicked();

    void on_save_Btn_clicked();

    void on_edit_Btn_clicked();

    void on_delete_Btn_clicked();

    void on_confirm_Btn_clicked();

    void on_close_Btn_clicked();

private:
    void init();
    void setBtnVisble(bool ok);
    //在左侧列表中添加项
    void addListWidgetItem(QString seq,QPoint point);
    void clearWidgetList();
private:
    Ui::TeachEdit *ui;
    QMap<QString,ST_ItemPoint*> m_map;
    QList<CListWidgetItem*> widgetList;
    QList<QListWidgetItem*> itemList;
};

#endif // TEACHEDIT_H
