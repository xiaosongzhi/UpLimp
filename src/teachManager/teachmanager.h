#ifndef TEACHMANAGER_H
#define TEACHMANAGER_H

#include <QWidget>
class QListWidgetItem;
class TeachEdit;
namespace Ui {
class TeachManager;
}

class TeachManager : public QWidget
{
    Q_OBJECT

public:
    explicit TeachManager(QWidget *parent = nullptr);
    ~TeachManager();
    //更新处方列表
    void updateRecipeList();

signals:
    //将选中的处方发送给训练管理类
    void signalSendSelectedRecipe(QString recipeName);
protected:
    virtual void showEvent(QShowEvent *event);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void slotSelectItemDoubleClicked(QListWidgetItem *item);

    void on_right_Btn_clicked();

    void on_left_Btn_clicked();

    void slotUpdateDatabase();

    void on_search_Btn_clicked();

    void on_search_LineEdit_textChanged(const QString &arg1);

private:
    void showUserData(uint16_t pageIndex);
    //加载样式表
    void loadStyleSheetFile();
private:
    Ui::TeachManager *ui;
    TeachEdit *m_teachEdit;

    int m_currentPage;
    int m_totalPage;
    int m_totalUserNum;
    QList<QVariantMap> m_recipeValueList;
};

#endif // TEACHMANAGER_H
