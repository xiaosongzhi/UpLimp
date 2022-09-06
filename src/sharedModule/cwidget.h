#ifndef CWIDGET_H
#define CWIDGET_H

#include <QWidget>
class QHBoxLayout;
class QVBoxLayout;
class CWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CWidget(QWidget *parent = nullptr);

    void setColumnCount(int);
    void setRowCount();

    int getColumnCount();
    int getRowCount();

    void addWidgets(QList<QWidget*> widgetList);
    void addWidget(QWidget* widget,int row,int column);
signals:
private:
    QHBoxLayout *m_HBoxLayoutUpper;
    QHBoxLayout *m_HBoxLayoutLower;
    QVBoxLayout *m_VBoxLayout;
    int m_rowCount;
    int m_columnCount;
    int m_totalWidgetCount;
};

#endif // CWIDGET_H
