#ifndef MOVEMENTRANGE_H
#define MOVEMENTRANGE_H

#include <QWidget>

namespace Ui {
class MovementRange;
}

class MovementRange : public QWidget
{
    Q_OBJECT

public:
    explicit MovementRange(QWidget *parent = nullptr);
    ~MovementRange();

private:
    Ui::MovementRange *ui;
};

#endif // MOVEMENTRANGE_H
