#ifndef DRAINTANKFORM_H
#define DRAINTANKFORM_H

#include <QWidget>

namespace Ui {
class DrainTankForm;
}

class DrainTankForm : public QWidget
{
    Q_OBJECT

public:
    explicit DrainTankForm(QWidget *parent = nullptr);
    ~DrainTankForm();

private:
    Ui::DrainTankForm *ui;
};

#endif // DRAINTANKFORM_H
