#ifndef SCOPEREPORT_H
#define SCOPEREPORT_H

#include <QWidget>

namespace Ui {
class ScopeReport;
}

class ScopeReport : public QWidget
{
    Q_OBJECT

public:
    explicit ScopeReport(QWidget *parent = nullptr);
    ~ScopeReport();
    //绘制范围
    void paintScopeTrack(QList<QPoint> &);
    //绘制当前用户的最大最小值
    void paintLimitValue();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent *event);
private slots:
    void on_delete_Btn_clicked();

    void on_save_Btn_clicked();

    void on_close_Btn_clicked();

private:
    //创建X轴柱状图
    void createXAxisBarChart(QList<QVariantMap>&);
    //创建Y轴柱状图
    void createYAxisBarChart(QList<QVariantMap>&);
private:
    Ui::ScopeReport *ui;
};

#endif // SCOPEREPORT_H
