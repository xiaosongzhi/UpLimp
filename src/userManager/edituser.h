#ifndef EDITUSER_H
#define EDITUSER_H

#include <QWidget>
#include "userdataformate.h"

namespace Ui {
class EditUser;
}

class EditUser : public QWidget
{
    Q_OBJECT

public:
    explicit EditUser(QWidget *parent = nullptr);
    ~EditUser();
    //0-新建用户  1-编辑用户
    void switchPageType(QString ID,int type = 0);

protected:
    virtual void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
signals:
    void signalUpdateUserList();
    /****完成用户选择,将页面切换至训练界面
     * @参数int type  0-返回用户管理界面  1-进入训练界面
     ******/
    void signalUserSelected(int type);
private slots:
    void on_delete_Btn_clicked();

    void on_modify_Btn_clicked();

    void on_confirm_Btn_clicked();

    void on_save_Btn_clicked();

    void on_leftLimp_Btn_clicked();

    void on_rightLimp_Btn_clicked();

    void on_allLimp_Btn_clicked();

    void on_male_RadioButton_clicked();

    void on_female_RadioButton_clicked();

    void on_close_Btn_clicked();

private:
    void addUserPage();
    void editUserPage(QString ID);
    //设置lineEdit的输入限制
    void setLineEditFormat();
    //设置用户信息
    void setUserData(ST_PatientMsg);
    //获取用户信息
    ST_PatientMsg getUserData();
    //清理用户数据
    void clearuserData();
    //初始化控件
    void initWidgetUI();

    //读写setting
    void writeSetting();
    void readSetting();

    void loadStyleSheetFile();

    void setTitile(QString title);
private:
    Ui::EditUser *ui;
    uint32_t m_currentID; //系统给用户分配的ID
};

#endif // EDITUSER_H
