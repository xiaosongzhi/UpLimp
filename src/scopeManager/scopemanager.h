#ifndef SCOPEMANAGER_H
#define SCOPEMANAGER_H

#include <QWidget>
class QMovie;
class SCopeTest;
namespace Ui {
class ScopeManager;
}

class ScopeManager : public QWidget
{
    Q_OBJECT

public:
    explicit ScopeManager(QWidget *parent = nullptr);
    ~ScopeManager();

    void startMovie();
    void stopMovie();

protected:

//    virtual void showEnvent(QShowEvent* event);

private slots:

    void on_startTest_Btn_clicked();
    void slotMovieFinished();

private:
    void loadStyleSheetFile();

    float getScopeArea();
private:
    Ui::ScopeManager *ui;
    SCopeTest *m_scopeTest;
    QMovie *m_scopeMovie;
};

#endif // SCOPEMANAGER_H
