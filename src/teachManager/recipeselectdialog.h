#ifndef RECIPESELECTDIALOG_H
#define RECIPESELECTDIALOG_H

#include <QWidget>
class QListWidgetItem;
namespace Ui {
class RecipeSelectDialog;
}

class RecipeSelectDialog : public QWidget
{
    Q_OBJECT

public:
    explicit RecipeSelectDialog(QWidget *parent = nullptr);
    ~RecipeSelectDialog();

    //更新处方列表
    void updateRecipeList();
    //获取处方名
    QString getSlectedRecipe();
signals:
    //将选中的处方发送给训练管理类
    void signalSendSelectedRecipe(QString recipeName);
private slots:
    void slotSelectItemDoubleClicked(QListWidgetItem *item);

    void on_right_Btn_clicked();

    void on_left_Btn_clicked();

    void slotUpdateDatabase();

    void on_search_Btn_clicked();

    void on_search_LineEdit_textChanged(const QString &arg1);
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
private:
    void showUserData(uint16_t pageIndex);
    //加载样式表
    void loadStyleSheetFile();
private:
    Ui::RecipeSelectDialog *ui;
    int m_currentPage;
    int m_totalPage;
    int m_totalUserNum;
    QString m_recipeName;
    QList<QVariantMap> m_recipeValueList;
};

#endif // RECIPESELECTDIALOG_H
