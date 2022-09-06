#ifndef RANKINGLIST_H
#define RANKINGLIST_H
#include <QWidget>
#include "userdataformate.h"

namespace Ui {
class RankingList;
}

class RankingList : public QWidget
{
    Q_OBJECT

public:
    explicit RankingList(QWidget *parent = nullptr);
    ~RankingList();
    //根据字段排行 0-训练时长 1-总得分
    void setSortByField(int);

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_previous_Btn_clicked();

    void on_next_Btn_clicked();

private:
    //刷新表格
    void flushTableData();
    //设置tableWidget的属性
    void initTableWidget();
    //在表中填充数据
    void fillTableWidget(ST_TrainRanking&,int row);
private:
    Ui::RankingList *ui;
    int m_currentPage;          //tablewidget当前的页码,从1开始
    const int m_perPageCount;   //每个页数显示的最大条数
    int m_currentUserRanking;   //当前用户的排名
    int m_totalPage;            //总页数
    int m_maxRow;               //最大行数
    int m_totalRowCount;        //总行数
    QList<QVariantMap> m_rankingListMap;
};

#endif // RANKINGLIST_H
